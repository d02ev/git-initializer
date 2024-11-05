package constants

const (
	GitUser = "GIT_USER"
	GitKey = "GIT_KEY"
	ListIgnoresArg = "lsig"
	ListAvailableCmdsArg = "lsc"
	IgnoreFlag = "--ig"
	KeyFlag = "--key"
	UserFlag = "--user"
	InitArg = "init"
	AddArg = "add"

)

const (
	GitApiUrl = "https://api.github.com/user/repos"
	GitHomePageBaseUrl = "https://github.com"
	GitApiAuthHeaderName = "Authorization"
	GitApiAuthHeaderValue = "Bearer"
	GitApiAcceptHeaderName = "Accept"
	GitApiAcceptHeaderValue = "application/vnd.github+json"
	GitHubApiVersionHeaderName = "X-GitHub-Api-Version"
	GitHubApiVersionHeaderValue = "2022-11-28"
	GitContentTypeHeaderName = "Content-Type"
	GitContentTypeHeaderValue = "application/json"
)

const (
	ErrorColor = "\033[31m"
	InfoColor = "\033[34m"
	SuccessColor = "\033[32m"
	Reset = "\033[0m"
)

const (
	DefaultIgnoreFiles = "dotenv,visualstudiocode,vs,linux"
	IgnoreFlagDefaults = "dotenv,visualstudiocode,vs,linux"
	IgnoreFileName = ".gitignore"
	ReadMeFileName = "README.md"
	StaticFileDirName = ".git-initializer"
	IgnoreJsonFileName = "ignores.json"
	ValidIgnoreListFileName = "valid_ignores_list.txt"
)

var ValidArgs = []string{"lsig", "lsc", "init", "add"}
var ValidFlags = []string{"--ig", "--key", "--user"}

const (
	HelpMessage = `
git-initializer: A CLI tool to manage the initial workflow of a new Git repository.

Usage:
	git-initializer <command> [options]

Commands:
	init		Initializes the current directory as a Git repository and creates a remote repository
					Usage: git-initializer init [--ig=dotenv,visualstudiocode,vs,linux]
					Options:
						--ig=<ignore_files>		Comma separated list of ignore files

	add			Adds Git API key and Git user to OS environment
					Usage: git-initializer add --key=GIT_KEY --user=GIT_USER
					Options:
						--key=<key>		Git API key for authentication
						--user=<user>	Git username for repository access

	lsig		Lists valid ignore files

Note:
	Please run: git-initializer add --key=<key> --user=<user> to add <key> and <user> before initializing a new repository

Examples:
	git-initializer init [--ig=dotenv,visualstudiocode,vs,linux]
	git-initializer add --key=GIT_KEY --user=GIT_USER
	`
)