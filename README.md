
# Sudoku Solver — C, MATLAB & LabVIEW

> End-to-end Sudoku solver: cell image recognition in C, recursive backtracking solver in MATLAB, and a parallel LabVIEW implementation.  
> EPFL, 2023.

---

## Overview

A full pipeline to solve a Sudoku from a scanned image:

```
Binary cell image (.bin)
        │
        ▼
  [C] Digit recognition        ← bitmap matching + white pixel ratio
        │
        ▼
  CellValue.txt                ← identified digit + confidence %
        │
        ▼
  [MATLAB] Sudoku solver       ← constraint propagation + backtracking
        │
        ▼
  Solution PDF                 ← formatted grid with metadata
```

A complete LabVIEW implementation (`.vi`) covers the same pipeline end-to-end.

---

## Part 1 — Digit recognition (`main.c`)

Reads a binary cell image and identifies which digit (0–9) it contains.

### Input

- A `.bin` file encoding a grayscale cell image: first 8 bytes = `int width` + `int height`, then `width × height` bytes of pixel data (0 = black, 1 = white)
- 10 threshold parameters passed via command line (one per digit 0–9)

```bash
./readcell S3C1_Cell.bin 80 75 75 75 75 75 75 75 75 75
#                        ^   ^  ...
#                   blank  digit 1  ... digit 9
```

### Algorithm

**Step 1 — Empty cell detection**  
Computes the ratio of white pixels across the whole cell. If above `seuil[0]`, the cell is empty → writes `d: '0'` to `CellValue.txt`.

```c
double ratio_pixels_blanc(Cell, width, height):
    white = (width × height) - sum(Cell)
    return white / (width × height) × 100
```

**Step 2 — Sliding window bitmap matching**  
For each digit 1–9, slides the 16×24 reference bitmap across the cell image and counts matching pixels at each position:

```
for digit in 1..9:
    for offset_col in 0..(width - 16):
        for offset_line in 0..(height - 24):
            count identical bits between bitmap and cell window
            keep MAX over all positions

    ratio = MAX / (16 × 24) × 100
    if ratio >= seuil[digit] and ratio > current best:
        digit_reconnu = digit
```

The digit with the **highest matching ratio** above its threshold wins.

**Step 3 — Bit extraction**  
The reference bitmaps are stored as `int16_t` rows (one per scan line). A single bit is extracted with:

```c
unsigned char GetDigitBitmapBit(int digit, int line, int col):
    return (DigitBitmap[digit-1][line] >> (15 - col)) & 1
```

### Output

Writes `CellValue.txt`:
```
d: '7', 84%
```

---

## Part 2 — Sudoku solver (MATLAB)

### Algorithm

Two-stage approach at each recursive call:

**Stage 1 — Constraint propagation (`FillHypothesis`)**  
Builds a `9×9×9` binary tensor `A` where `A(row, col, k) = 1` means digit `k` is still a valid candidate for cell `(row, col)`. Known values propagate constraints across rows, columns, and 3×3 blocks.

**Stage 2 — Backtracking with MRV heuristic (`SolveSudoku`)**  
When propagation alone stalls, picks the most constrained cell and tries candidates recursively:

```
SolveSudoku(M, rlevel):
    A = FillHypothesis(M)
    s = checkSolved(A)       # -1 = contradiction, 0 = in progress, 1 = solved

    if s == 1  → fill remaining forced cells, return
    if s == -1 → restore M from saved state, backtrack
    if s == 0:
        (x, y, choices) = getChoices(A, M)   # MRV: fewest candidates
        for each candidate:
            M[y,x] = candidate
            SolveSudoku(M, rlevel + 1)
            if solved → return
```

**Cell selection — MRV heuristic (`getChoices`)**

```matlab
R = sum(A, 3)         % count valid candidates per cell
R(filled_cells) = 10  % exclude already-filled cells
[~, p] = min(R)       % cell with fewest options
```

### Files

```
├── SolveSudoku.m     # Core recursive solver
├── getChoices.m      # MRV heuristic
├── checkSolved.m     # Checks solution state (-1 / 0 / 1)
├── dispSudoku.m      # Renders solution to PDF
├── Solve.m           # Entry point — valid puzzle
└── SolveNotOK.m      # Entry point — unsolvable puzzle (error handling demo)
```

### Usage

```matlab
Solve          % run on a valid puzzle
SolveNotOK     % demonstrate handling of an unsolvable grid
```

Grid format — `0` = empty cell:
```matlab
M = [0,5,0,4,0,0,7,0,0;
     0,6,0,0,0,8,0,2,0;
     ...
     0,0,5,1,8,0,0,0,3];
```

### Performance

| Puzzle | Iterations | Max recursion depth | Time |
|---|---|---|---|
| Example 1 | 47 | 47 | 0.084s |
| Example 2 | 103 | 51 | 0.086s |

### Output

PDF with the solved grid — **bold** = given digits, *italic* = solved digits, metadata printed below (iteration count, recursion depth, solve time, timestamp).

---

## Part 3 — LabVIEW implementation

The same full pipeline is implemented in LabVIEW:

| File | Role |
|---|---|
| `sudokufinal.vi` | Top-level VI — runs the full solver |
| `computecell.vi` | Cell processing subVI |
| `readCellValue.vi` | Reads and parses `CellValue.txt` |

---

## Stack

| Tool | Role |
|---|---|
| C (C99) | Cell image reading, digit recognition |
| MATLAB | Constraint propagation, backtracking solver, PDF output |
| LabVIEW | Full alternative implementation |

---

**Course:** EPFL, 2023
