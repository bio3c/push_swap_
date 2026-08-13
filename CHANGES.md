# push_swap — Changes & Explanations

This document explains every change made during the code review of the
`push_swap` project: what was found, why each change was made, the exact code
before/after, and the test results.

---

## 1. Summary (for the project partner)

We reviewed the whole repository against the Norm, the subject, and the
peer-review checklist, then stress-tested it (all 4 strategies, sizes 2–500,
negative numbers, INT_MIN/INT_MAX, valgrind). Three concrete problems were found
and fixed:

1. **Default (`--adaptive`) sorting was terrible on tiny inputs.**
   3 numbers cost up to **10 ops** and 5 numbers up to **25 ops**, because
   `--adaptive` routed everything by "disorder" into radix/selection sort, which
   are overkill for 2–5 elements. The peer review wants 3 numbers ≤ 5 ops and
   5 numbers ≤ 15 ops. A dedicated hardcoded sorter for `n ≤ 5` was added inside
   `sort_adaptive` (the default path). Now 3 numbers cost **≤ 2 ops** and
   5 numbers **≤ 10 ops**.

2. **A global variable existed**, which the subject forbids. `operations/opcount.c`
   had `static t_opcount *g_counter;`. It was removed and replaced by threading a
   `t_opcount *` pointer explicitly through every operation and sort function.

3. **`--bench` printed nothing for already-sorted input.** Now it prints
   `disorder: 0.00%`, the strategy, and `total: 0`, as the peer review expects.

Everything still compiles with `-Wall -Wextra -Werror`, passes norminette, has no
leaks, and the 100/500-number performance is unchanged (still "good"). The forced
`--simple`/`--medium`/`--complex` flags were left as their pure algorithms.

---

## 2. The three fixes and the reasoning

### Fix A — Remove the global variable (thread the counter)

**Why:** The subject says *"Global variables are forbidden"*. `opcount.c` used a
file-scope `static t_opcount *g_counter;` plus two functions `set_opcount_ptr()`
and `inc_opcount()`. `main` had to "install" the pointer before sorting and
"uninstall" it after — fragile, and technically a global.

**Approach:** Delete the global and `set_opcount_ptr`. Pass `t_opcount *c` as an
explicit argument down the whole call chain: `main → sort functions → operations`.

### Fix B — Small-input optimization (only for `--adaptive`/default)

**Why:** The peer-review op-count thresholds (3 → ≤ 5, 5 → ≤ 15) only apply to the
**default** run (no flag). The default is `--adaptive`. Its disorder routing sent
tiny inputs into radix sort (10 ops for 3 numbers, 25 for 5). The fix:
`sort_adaptive` short-circuits `n ≤ 5` into a dedicated `sort_small` that does the
classic "sort 3 by hand, push smallest to B for 4/5".

**Why only `--adaptive`?** The subject requires *four distinct strategies*; forcing
`--simple`/`--medium`/`--complex` must still run their real algorithms (and the
peer review only checks those for correctness, not op count).

### Fix C — Benchmark on sorted input

**Why:** `sort_stack` did `if (is_sorted(a)) return;` *before* any bench output, so
`./push_swap --bench 1 2 3 4 5` printed nothing. The peer review expects
`disorder: 0.00%` for sorted input.

**Approach:** Compute `disorder` (and bench info) *before* the sorted check, and
emit bench output in the sorted branch too.

---

## 3. Line-by-line changes

### 3.1 `operations/opcount.c` (core of Fix A)

**Before:**

```c
static t_opcount	*g_counter;

void	set_opcount_ptr(t_opcount *c)
{
	g_counter = c;
}

void	inc_opcount(int type)
{
	if (!g_counter)
		return ;
	if (type == OP_SA)
		g_counter->sa++;
	/* ... etc ... */
}
```

**After:**

```c
void	inc_opcount(t_opcount *c, int type)
{
	if (!c)
		return ;
	if (type == OP_SA)
		c->sa++;
	else if (type == OP_SB)
		c->sb++;
	else if (type == OP_SS)
		c->ss++;
	else if (type == OP_PA)
		c->pa++;
	else if (type == OP_PB)
		c->pb++;
	else if (type == OP_RA)
		c->ra++;
	else if (type == OP_RB)
		c->rb++;
	else if (type == OP_RR)
		c->rr++;
	else if (type == OP_RRA)
		c->rra++;
	else if (type == OP_RRB)
		c->rrb++;
	else if (type == OP_RRR)
		c->rrr++;
}
```

- `static t_opcount *g_counter;` — **deleted** (this was the global).
- `set_opcount_ptr()` — **deleted** entirely.
- `void inc_opcount(t_opcount *c, int type)` — receives the counter as a normal
  parameter instead of reading a global.
- `if (!c)` — null-guard replacing `if (!g_counter)`.
- Every `g_counter->X++` becomes `c->X++`.

### 3.2 `operations/operations.h` (prototypes follow the new signatures)

```c
void	inc_opcount(t_opcount *c, int type);
void	swap(t_stack *s);                          /* unchanged (internal helper) */
int		sa(t_opcount *c, t_stack *a);              /* + c */
int		sb(t_opcount *c, t_stack *b);              /* + c */
int		ss(t_opcount *c, t_stack *a, t_stack *b);  /* + c */
int		pa(t_opcount *c, t_stack *a, t_stack *b);  /* + c */
int		pb(t_opcount *c, t_stack *b, t_stack *a);  /* + c */
void	rotate(t_stack *s);                        /* unchanged */
int		ra(t_opcount *c, t_stack *a);              /* + c */
int		rb(t_opcount *c, t_stack *b);              /* + c */
int		rr(t_opcount *c, t_stack *a, t_stack *b);  /* + c */
void	rev_rotate(t_stack *s);                    /* unchanged */
int		rra(t_opcount *c, t_stack *a);             /* + c */
int		rrb(t_opcount *c, t_stack *b);             /* + c */
int		rrr(t_opcount *c, t_stack *a, t_stack *b); /* + c */
```

- `set_opcount_ptr` prototype removed.
- The internal helpers `swap`, `rotate`, `rev_rotate` keep their signature — they
  do not count operations, only the public wrappers do.
- `c` is placed first so every caller reads consistently as `op(c, stack…)`.

### 3.3 `operations/swap.c`, `push.c`, `rotate.c`, `rev_rotate.c`

Each public operation changes in the same mechanical way. Example `sa`:

**Before:**

```c
int	sa(t_stack *a)
{
	int	count_sa;

	count_sa = 1;
	if (!a || a->size < 2)
		return (0);
	swap(a);
	inc_opcount(OP_SA);
	write(1, "sa\n", 3);
	return (count_sa);
}
```

**After:**

```c
int	sa(t_opcount *c, t_stack *a)
{
	int	count_sa;

	count_sa = 1;
	if (!a || a->size < 2)
		return (0);
	swap(a);
	inc_opcount(c, OP_SA);   /* pass c */
	write(1, "sa\n", 3);
	return (count_sa);
}
```

Only two things change per function: the **signature gains `t_opcount *c`**, and
**`inc_opcount(OP_X)` → `inc_opcount(c, OP_X)`**. Same for `sb`, `ss`, `pa`, `pb`,
`ra`, `rb`, `rr`, `rra`, `rrb`, `rrr`. The operation logic is untouched.

### 3.4 `algorithms/algorithms.h` (sort prototypes gain `t_opcount *c`)

```c
int	sort_simple(t_stack *a, t_stack *b, t_opcount *c);
int	sort_complex(t_stack *a, t_stack *b, t_opcount *c);
int	sort_medium(t_stack *a, t_stack *b, t_opcount *c);
int	process_chunk(t_stack *a, t_stack *b, t_chunk *chunk, t_opcount *c);
int	push_stack_a(t_stack *b, int current_index, t_opcount *c);
int	sort_adaptive(t_stack *a, t_stack *b, t_opcount *c);
```

Important detail — `process_chunk` changed from
`(…, int start, int end, t_opcount *c)` (5 args, **Norm violation**: max 4) to
`(…, t_chunk *chunk, t_opcount *c)` (4 args). It now reads `chunk->start` /
`chunk->end` internally.

### 3.5 `sort_simple.c`, `sort_complex.c`, `sort_medium.c`, `medium_a_utils.c`, `medium_b_utils.c`

Same mechanical threading — every call site forwards `c`:

- `sort_simple`: `ra(c,a)`, `rra(c,a)`, `pb(c,b,a)`, `pa(c,a,b)`.
- `sort_complex` / `process_bit`: `pb(c,b,a)`, `ra(c,a)`, `pa(c,a,b)`.
- `sort_medium`: `process_chunk(a,b,&chunk,c)` and `move_b_to_a(a,b,c)`.
- `medium_a_utils`: `move_chunk_to_top_a(a, start, end, c)`; `process_chunk` uses
  `chunk->start`/`chunk->end`.
- `medium_b_utils`: `rb(c,b)`, `rrb(c,b)`.

No algorithm logic changed — only argument plumbing.

### 3.6 `algorithms/sort_adaptive.c` (Fix B — the new small-input sorter)

Three new `static` helpers were added plus a guard at the top of `sort_adaptive`.

**`sort_three` (exactly 3 elements, ≤ 2 ops):**

```c
static void	sort_three(t_stack *a, t_opcount *c)
{
	int	top;
	int	mid;
	int	bot;

	top = a->top->index;
	mid = a->top->next->index;
	bot = a->top->prev->index;
	if (top > mid && mid < bot && top < bot)      /* [1,0,2] -> sa        */
		sa(c, a);
	else if (top > mid && mid > bot)              /* [2,1,0] -> sa, rra   */
	{
		sa(c, a);
		rra(c, a);
	}
	else if (top > mid && mid < bot && top > bot) /* [2,0,1] -> ra        */
		ra(c, a);
	else if (top < mid && mid > bot && top < bot) /* [0,2,1] -> sa, ra    */
	{
		sa(c, a);
		ra(c, a);
	}
	else if (top < mid && mid > bot && top > bot) /* [1,2,0] -> rra       */
		rra(c, a);
}
```

- `top`, `mid`, `bot` are the three **indices** (0 = smallest, 2 = largest) of the
  top / next / bottom nodes. `index_stack()` already ran in `main`, so `.index`
  is valid.
- It is a **case analysis of the 5 non-sorted permutations** of 3 elements; the
  6th (`[0,1,2]`) is already sorted and skipped. Each case uses at most 2
  operations (verified against `checker_linux` for all 6 permutations).

**`sort_five` (4 or 5 elements):**

```c
static void	sort_five(t_stack *a, t_stack *b, t_opcount *c)
{
	int	min_pos;
	int	rot;

	while (a->size > 3)
	{
		min_pos = stack_min_pos(a);
		if (min_pos <= a->size / 2)
		{
			rot = min_pos;
			while (rot--)
				ra(c, a);
		}
		else
		{
			rot = a->size - min_pos;
			while (rot--)
				rra(c, a);
		}
		pb(c, b, a);
	}
	sort_three(a, c);
	while (b->size > 0)
		pa(c, a, b);
}
```

- While more than 3 elements remain in A, find the minimum (by index), rotate it
  to the top using the **shorter direction** (`ra` if top half, `rra` otherwise),
  and push it to B.
- 5 elements → 2 pushes; 4 elements → 1 push.
- Then sort the remaining 3 in A with `sort_three`, and push everything back.

**`sort_small` (dispatcher):**

```c
static void	sort_small(t_stack *a, t_stack *b, t_opcount *c)
{
	if (a->size == 2)
	{
		if (a->top->index > a->top->next->index)
			sa(c, a);
	}
	else if (a->size == 3)
		sort_three(a, c);
	else
		sort_five(a, b, c);
}
```

- size 2 → one `sa` if out of order.
- size 3 → `sort_three`.
- size 4/5 → `sort_five`.

**Changed `sort_adaptive`:**

```c
int	sort_adaptive(t_stack *a, t_stack *b, t_opcount *c)
{
	double	d;

	if (a->size <= 5)
	{
		sort_small(a, b, c);
		return (0);
	}
	d = compute_disorder(a);
	if (d < 0.2)
		sort_simple(a, b, c);
	else if (d < 0.5)
		sort_medium(a, b, c);
	else
		sort_complex(a, b, c);
	return (0);
}
```

- The `if (a->size <= 5)` guard is the **only** behavior change for the adaptive
  strategy; the disorder routing is untouched and now only runs for `n ≥ 6`.
- `return (0)` right after `sort_small` prevents falling into the disorder path.

### 3.7 `main.c` (Fixes A + C)

**New helper — `run_strategy` (extracted so `sort_stack` stays under 25 lines):**

```c
static void	run_strategy(t_stack *a, t_stack *b, t_strategy strat, t_opcount *c)
{
	index_stack(a);
	if (strat == SIMPLE)
		sort_simple(a, b, c);
	else if (strat == MEDIUM)
		sort_medium(a, b, c);
	else if (strat == COMPLEX)
		sort_complex(a, b, c);
	else
		sort_adaptive(a, b, c);
}
```

- Calls `index_stack(a)` once, then dispatches to the chosen strategy, forwarding
  the counter `c`.
- Extracted because adding the bench/sorted logic made `sort_stack` exceed the
  Norm's 25-line limit. (Norm also caps a `.c` file at 5 functions; `main.c` now
  has exactly 5: `is_sorted`, `free_args`, `run_strategy`, `sort_stack`, `main`.)

**Changed `sort_stack`:**

```c
static void	sort_stack(t_stack *a, t_stack *b, t_strategy strat, int bench)
{
	double		disorder;
	t_opcount	opc;

	opc = (t_opcount){0};
	disorder = 0.0;
	if (bench)
		disorder = compute_disorder(a);
	if (is_sorted(a))
	{
		if (bench)
		{
			bench_disorder(disorder);
			bench_strategy(strat, disorder, &opc);
		}
		return ;
	}
	run_strategy(a, b, strat, &opc);
	if (bench)
	{
		bench_disorder(disorder);
		bench_strategy(strat, disorder, &opc);
	}
}
```

- `disorder` is now computed **before** the sorted check, so it can be reported
  even when nothing needs sorting.
- The `if (is_sorted(a))` early-return **still exists**, so
  `./push_swap 42` / `./push_swap 0 1 2 3` still print nothing to stdout. Only
  difference: inside this branch, if `--bench` was passed, the bench report is
  emitted to stderr, then return.
- `run_strategy(a, b, strat, &opc);` replaces the old
  `set_opcount_ptr(&opc); if/else… set_opcount_ptr(NULL);` block — the global
  install/uninstall is gone.
- The trailing `if (bench) { … }` block is unchanged.

---

## 4. Before / after numbers

| input | flag | before | after |
|-------|------|--------|-------|
| 3 numbers (worst) | default | 10 ops | **2 ops** |
| 5 numbers (worst) | default | 25 ops | **10 ops** |
| `--bench 1 2 3 4 5` | — | (no output) | `disorder: 0.00%` + `total: 0` |
| global variable | — | `g_counter` present | **removed** |
| norminette | — | OK | **OK** |
| 100 / 500 perf | default | ~783–1084 / ~6784–7459 | **unchanged** |

The forced `--simple` (3 → ≤ 8, 5 → ≤ 16), `--medium` (3 → ≤ 9, 5 → ≤ 18), and
`--complex` (3 → ≤ 10, 5 → ≤ 25) are intentionally unchanged — they run their real
algorithms and stay correct; only their op count on tiny inputs is higher, which the
peer review does not penalize for forced flags.

---

## 5. Test results

- `make re` with `-Wall -Wextra -Werror`: clean, no warnings.
- `norminette`: all files OK.
- Correctness: 2–500 elements × 4 strategies × multiple runs → checker `OK`,
  0 failures.
- Negatives + INT_MIN/INT_MAX mixes across all flags → 0 failures.
- Small inputs, exhaustive (all permutations):
  - 3 numbers (6 perms): `--adaptive` ≤ 2 ops, forced flags 8–10, all `OK`.
  - 4 numbers (24 perms): `--adaptive` ≤ 6 ops.
  - 5 numbers (120 perms): `--adaptive` ≤ 10 ops.
- 100 numbers: ~783–1084 ops (good; occasionally near excellent < 700).
- 500 numbers: ~6784–7459 ops (good < 8000).
- valgrind: 0 errors, 0 leaks on sorted / normal / error / large inputs.

---

## 6. Disorder analysis — which strategy does the default actually pick?

A subtle but important point. The op-count limits (100 → < 2000, 500 → < 12000)
are only tested by the peer review using **random** `shuf` input with **no flag**,
i.e. the default `--adaptive` strategy. The disorder (normalized inversion count)
of a random permutation is tightly concentrated around **0.5**:

- Expected inversions = `n(n-1)/4`, total pairs = `n(n-1)/2` → expected
  disorder = `0.5`.
- Standard deviation ≈ `1/(6√n)` (≈ 0.75% at n = 500).

Measured on random input:

```
n=100:  disorder 46–55%  →  medium O(n√n) or complex O(n log n)
n=500:  disorder 48–52%  →  medium O(n√n) or complex O(n log n)
```

Therefore the default path routes random inputs to **medium or complex** (both
well under 12000 ops). It **never** routes them to `simple` O(n²), because
`simple` is only chosen when `disorder < 0.2` (nearly-sorted input), which a random
shuffle essentially never produces — and that low-disorder → O(n²) mapping is
exactly what the subject mandates.

| regime | disorder | strategy | 500-number ops |
|--------|----------|----------|----------------|
| low | `< 0.2` | `simple` O(n²) | ~32k (only nearly-sorted input, never `shuf`) |
| medium | `0.2–0.5` | `medium` O(n√n) | ~7.4k |
| high | `≥ 0.5` | `complex` O(n log n) | ~6.8k |

---

## 7. Performance benchmark vs. subject targets

| algorithm | 100 numbers | vs. limit | 500 numbers | vs. limit |
|-----------|-------------|-----------|-------------|-----------|
| `--simple` | ~1390–1520 | < 1500 good | ~30k–33k | over 12k (O(n²), not benchmarked) |
| `--medium` | ~730–800 | < 1500 good | ~7.4k–7.5k | < 8000 good |
| `--complex` | ~1084 | < 1500 good | ~6.8k | < 8000 good |
| `--adaptive` (default) | ~750–1084 | < 1500 good | ~6.8k–7.4k | < 8000 good |

The `--simple` O(n²) figure at 500 numbers is expected — it cannot reach 12k while
remaining a genuine O(n²) selection sort (rotation cost alone is ≈ n²/8). It is
not part of any graded benchmark (peer review only checks forced flags for
correctness on 5 numbers, and benchmarks 100/500 with the default adaptive).

---

## 8. Files modified

```
main.c
algorithms/algorithms.h
algorithms/sort_adaptive.c
algorithms/sort_simple.c
algorithms/sort_complex.c
algorithms/sort_medium.c
algorithms/medium_a_utils.c
algorithms/medium_b_utils.c
operations/operations.h
operations/opcount.c
operations/swap.c
operations/push.c
operations/rotate.c
operations/rev_rotate.c
```
