# lmac

macOS CLI tool for managing Claude CLI and desktop installations, querying GitHub, and generating shell integrations.

## Commands

- [`claude`](#claude) — combined overview of Desktop and CLI installations
- [`claude-cli`](#claude-cli) — manage Claude CLI configs, data directories, and credentials
- [`claude-desk`](#claude-desk) — manage Claude desktop installations
- [`gh`](#gh) — GitHub utilities
- [`help`](#help) — show help overview or help for a specific command
- [`shell`](#shell) — generate shell completions

---

## claude

Unified view and switcher across Desktop and CLI profiles.

### claude list

```sh
lmac claude list
```

```
  profile  desktop    cli
  -------  ---------  ----------
  gc       Claude-gc  .claude-gc
  jl       Claude-jl  .claude-jl
▶ ps       Claude-ps  .claude-ps
```

Single table grouping Desktop and CLI installations by suffix. The active profile is marked with `▶`.

### claude choose

Interactively switch both Desktop and CLI to the same profile in one step:

```sh
lmac claude choose
```

```
Claude         → Claude-ps
~/.claude      → .claude-ps
~/.claude.json → .claude.json-ps
────────────────────────────────────────────────────────────
  gc   Claude-gc    .claude-gc
  jl   Claude-jl    (no cli)
▶ ps   Claude-ps    .claude-ps
```

Press **Enter** to confirm. Press **Esc** or **Ctrl-C** to abort without any changes.

On confirmation, for the selected suffix:
1. If `Claude-{suffix}` exists → `~/Library/Application Support/Claude` symlink is updated
2. If `.claude-{suffix}` exists → `~/.claude`, `~/.claude.json` symlinks are updated and the `"Claude Code-credentials"` keychain item is refreshed

Profiles that only exist on one side show `(no desktop)` or `(no cli)` in the menu.

---

## claude-cli

Manage the files and keychain credentials associated with multiple Claude CLI profiles.

Profile data lives in `~` as dotfiles with a shared suffix (e.g. `gc`, `ps`, `rm`):

| File | Purpose |
|------|---------|
| `~/.claude-{suffix}` | Claude CLI data directory |
| `~/.claude.json-{suffix}` | Claude CLI config file |
| `~/.claude-creds-{suffix}` | OAuth credentials for this profile |
| `~/.claude` | Active symlink → current `~/.claude-{suffix}` |
| `~/.claude.json` | Active symlink → current `~/.claude.json-{suffix}` |

### claude-cli list

Show all Claude CLI profiles with the active symlink targets:

```sh
lmac claude-cli list
```

```
~/.claude      → .claude-ps
~/.claude.json → .claude.json-ps

  gc   .claude-gc   .claude.json-gc
  jl   .claude-jl   .claude.json-jl
▶ ps   .claude-ps   .claude.json-ps
```

Active symlink targets are shown at the top. The active profile is marked with `▶`.

### claude-cli choose

Interactively switch to a different profile. Shows current symlink targets, then an arrow-key menu:

```sh
lmac claude-cli choose
```

```
~/.claude      → .claude-ps
~/.claude.json → .claude.json-ps
────────────────────────────────────────────────────────────
  gc  (.claude-gc, .claude-creds-gc, .claude.json-gc)
▶ ps  (.claude-ps, .claude-creds-ps, .claude.json-ps)
```

Press **Enter** to confirm. Press **Esc** or **Ctrl-C** to abort without any changes.

On confirmation, three things happen atomically:

1. `~/.claude` is replaced to point to `~/.claude-{suffix}`
2. `~/.claude.json` is replaced to point to `~/.claude.json-{suffix}`
3. The `"Claude Code-credentials"` keychain item is updated with the content of `~/.claude-creds-{suffix}`

### claude-cli show-creds

Print the current `"Claude Code-credentials"` keychain value to stdout:

```sh
lmac claude-cli show-creds
```

Useful for inspection or piping into other tools:

```sh
lmac claude-cli show-creds | jq '.claudeAiOauth.accessToken'
```

---

## claude-desk

List or interactively choose a Claude desktop installation from `~/Library/Application Support/`.

```sh
lmac claude-desk list         # list all installations with active marked ▶
lmac claude-desk choose       # interactive arrow-key selection
```

`list` shows the active symlink target at the top, then all versioned installations:

```
Claude → Claude-ps

▶ Claude-ps
  Claude-gc
  Claude-jl
```

`choose` shows the current `Claude →` target at the top, then a menu of versioned `Claude-*` entries. Press **Enter** to confirm, **Esc** or **Ctrl-C** to abort. On confirmation it replaces the `~/Library/Application Support/Claude` symlink to point to the chosen installation.

---

## gh

GitHub utilities. Requires the [GitHub CLI (`gh`)](https://cli.github.com/) installed and authenticated.

```sh
lmac gh help
```

### gh sm

Summarize your own pull requests or issues within a date range.

```sh
lmac gh sm <pr|issue> [options]
```

| Option | Short | Default | Description |
|--------|-------|---------|-------------|
| `--date` | `-d` | today | Reference date in `YYYY-MM-DD` format |
| `--months` | `-m` | `1` | How many months back the range starts |

The date range is computed as:
- **start** — first day of `date.month - months`
- **end** — first day of `date.month + 1`

**Examples:**

```sh
# PRs from the last month (default)
lmac gh sm pr

# Issues from 2 months back relative to a specific date
lmac gh sm issue --date 2026-01-15 --months 2

# Short flags
lmac gh sm pr -d 2026-03-01 -m 3
```

Outputs raw JSON from `gh search`, suitable for piping:

```sh
lmac gh sm pr | jq '.[].title'
```

### gh search

Search pull requests or issues across a GitHub owner (org or user) within a date range.

```sh
lmac gh search <prs|issues> --owner <owner> [options]
```

| Option | Short | Default | Description |
|--------|-------|---------|-------------|
| `--owner` | `-o` | *(required)* | GitHub org or user to search within |
| `--date` | `-d` | today | Reference date in `YYYY-MM-DD` format |
| `--months` | `-m` | `1` | How many months back the range starts |

**Examples:**

```sh
# PRs in an org from the last month
lmac gh search prs --owner my-org

# Issues in a user's repos from 3 months back
lmac gh search issues --owner octocat --months 3

# Specific date range
lmac gh search prs --owner my-org --date 2026-01-01 --months 2
```

Outputs raw JSON from `gh search`, suitable for piping:

```sh
lmac gh search prs --owner my-org | jq '.[] | {title, url}'
```

**Difference between `sm` and `search`:**

| | `sm` | `search` |
|-|------|----------|
| Scope | your PRs/issues (`author:@me`) | all PRs/issues in an owner |
| Required args | none | `--owner` |
| JSON fields | `number, title, createdAt, repository, body` | `title, body, state, url, assignees, repository, labels, commentsCount, updatedAt, createdAt` |

---

## help

Show the full command overview or delegate to a specific command:

```sh
lmac help                  # full overview
lmac help claude-cli       # claude-cli subcommand list
lmac help claude-desk      # claude-desk subcommand list
lmac help gh               # gh subcommand list
lmac help gh sm            # gh sm options
lmac help gh search        # gh search options
lmac help shell            # shell options
```

Running `lmac` with no arguments is equivalent to `lmac help`.

---

## shell

Generate shell completion scripts.

```sh
lmac shell --format <fish|zsh|bash>
lmac shell -f fish
```

**Fish** (fully implemented):

```sh
# Add to ~/.config/fish/config.fish
lmac shell --format fish | source
```

> `zsh` and `bash` formats are reserved but not yet implemented.

---

## License

[AGPL-3.0-or-later](LICENSE)
