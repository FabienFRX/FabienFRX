# TODO

## Tests
- [ ] Unit tests: expand coverage for Parser, AST, Evaluator.
	- Parser: invalid tokens, empty list error, nested lists, negative numbers, symbols with special chars.
	- AST conversion: special-forms errors (empty list, bad `define`/`lambda` shapes).
	- Evaluator: `+ - * div mod eq? <` happy paths; division/modulo by zero; non-boolean `if`; calling non-procedure; arity mismatch; unbound variable.
- [ ] CLI/IO tests:
	- Run a file, verify stdout and exit code 0.
	- Error paths return exit code 84 and print error to stderr.
	- `-i` REPL flag; stdin (non-TTY) mode behaves like file mode.
	- Usage message on invalid args.
- [ ] Integration tests (golden tests): evaluate programs in `examples/` and compare outputs to expected snapshots.
- [ ] Coverage: enable `stack test --coverage`, publish HTML as artifact, and enforce a minimum (e.g., 80%).
- [ ] Optional: property tests (QuickCheck) for arithmetic builtins and small-step equivalences.

## CI/CD
- [ ] Add GitHub Actions workflow `.github/workflows/ci.yml`:
	- Triggers on `push` and `pull_request`.
	- Setup GHC + Stack; cache Stack to speed builds.
	- Jobs: build, format check (Fourmolu/Ormolu), lint (HLint), test with coverage.
	- Upload `coverage` report and test logs as artifacts.
- [ ] Add static analysis & formatting:
	- `hlint` with a basic `hlint.yaml` (treat warnings as errors in CI).
	- `fourmolu` or `ormolu` with a `make fmt` and `make fmt-check` target.
- [ ] Coverage reporting to Codecov (optional): upload coverage from CI; add badge.
- [ ] Release workflow `.github/workflows/release.yml`:
	- Trigger on tags like `v*`.
	- Build Linux binary with Stack; upload as GitHub Release asset.
	- Attach example programs and a short CHANGELOG excerpt.
- [ ] Badges in README: CI status, coverage, latest release.

## Implementation Roadmap
- [ ] Bugfix: in `Evaluator.hs`, fix `ASymbol` lookup (stray characters after `lookup s env`).
- [ ] Error UX: harmonize error messages with project spec; ensure consistent prefixes (e.g., `*** ERROR:`) everywhere.
- [ ] Builtins: add `<=`, `>`, `>=`, `and`, `or`, `not`.
- [ ] Lists: add `cons`, `car`, `cdr`, and `null?` with parser/evaluator support.
- [ ] Quoting: support `'x` and `(quote x)` in Parser/AST/Evaluator.
- [ ] Multi-expression bodies: allow multiple forms in `lambda`/`define` bodies (e.g., via a `begin` special-form or sequence node) and evaluate in order.
- [ ] Equality: extend `eq?` to handle all value types with well-defined semantics.
- [ ] Documentation: improve README with usage, examples, error conventions; and PDF official doc.

## Developer Ergonomics
- [ ] Makefile targets: `build`, `test`, `fmt`, `fmt-check`, `lint`, `ci-local`.
- [ ] Pre-commit hooks: run format, lint, and tests on staged files.
- [ ] Devcontainer/Nix (optional): reproducible dev environment.

## Acceptance Checklist
- [ ] CI green on PRs with cached builds under ~3–5 minutes.
- [ ] Test coverage ≥ 80% with key paths exercised (Parser, Evaluator, CLI).
- [ ] Release pipeline produces a runnable `glados` binary for Linux.
- [ ] README documents how to build, test, and run locally and in CI.

