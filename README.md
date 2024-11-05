# git-initializer

A CLI tool to manage the initial workflow for Git repositories.

## Usage

```bash
git-initializer init [--ig=ignore_files]
git-initializer add --key=GIT_KEY --user=GIT_USER
```

## Commands

### init

Initializes the current directory as a Git repository and creates a remote repository with the same name.

```bash
git-initializer init [--ig=ignore_files]
```

### add

Adds the `GIT_API_KEY` and `GIT_USER` to the OS environment.

```bash
git-initializer add --key=GIT_KEY --user=GIT_USER
```

### lsig

Lists all the valid ignore files for the `--ig` flag.

```bash
git-initializer lsig
```

## Options

### --ig

Comma separated list of ignore files.

```bash
--ig=ignore_files
```

### --key

Git API key for authentication.

```bash
--key=GIT_KEY
```

### --user

Git username for authentication.

```bash
--user=GIT_USER
```

## Note

Please run: `git-initializer add --key=GIT_API_KEY --user=GIT_USER to add GIT_API_KEY and GIT_USER before initializing a new repository`

`GIT_API_KEY` can be found under `Settings > Developer Settings > Personal access tokens > <Generate new token>` in your GitHub account.

## Installation

- `curl`

  ```bash
  curl -sSL https://raw.githubusercontent.com/d02ev/git-initializer/main/install.sh | bash
  ```

- `wget`

  ```bash
  wget -O - https://raw.githubusercontent.com/d02ev/git-initializer/main/install.sh | bash
  ```
