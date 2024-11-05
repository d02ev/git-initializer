package utils

import (
	"bufio"
	"bytes"
	"encoding/json"
	"fmt"
	"net/http"
	"os"
	"os/exec"
	"github.com/d02ev/git-initializer/constants"
	"github.com/d02ev/git-initializer/errors"
	"github.com/d02ev/git-initializer/helpers"
	"github.com/d02ev/git-initializer/types"
)

type Utils struct {}

func (u Utils) HasCorrectArgCount(arg_name string, arg_count int) error {
	switch arg_name {
		case constants.AddArg:
			if arg_count < 3 {
				return &cerrors.InvalidArgCountError{ CountDirection: -1, ArgName: arg_name }
			}
			if arg_count > 4 {
				return &cerrors.InvalidArgCountError{ CountDirection: 1, ArgName: arg_name }
			}
			return nil;
		case constants.InitArg:
			if arg_count > 3 {
				return &cerrors.InvalidArgCountError{ CountDirection: 1, ArgName: arg_name }
			}
			return nil;
		case constants.ListIgnoresArg:
			if arg_count > 2 {
				return &cerrors.InvalidArgCountError{ CountDirection: 1, ArgName: arg_name }
			}
			return nil;
		case constants.ListAvailableCmdsArg:
			if arg_count > 2 {
				return &cerrors.InvalidArgCountError{ CountDirection: 1, ArgName: arg_name }
			}
			return nil;
	}

	return nil
}

func (u Utils) HasEmptyArgValue(arg_name string, arg_val string) error {
	if arg_val == "" {
		return &cerrors.EmptyArgValueError{ ArgName: arg_name }
	}
	return nil;
}

func (u Utils) InvalidArgName(arg_name string) error { 
	for _, arg := range constants.ValidArgs {
		if arg_name == arg {
			return nil;
		}
	}

	return &cerrors.InvalidArgError{ ArgName: arg_name }
}

func (u Utils) InvalidFlagName(flag_name string) error {
	for _, flag := range constants.ValidFlags {
		if flag_name == flag {
			return nil;
		}
	}

	return &cerrors.InvalidFlagError{ FlagName: flag_name }
}

func (u Utils) InvalidIgnoreFileValue(arg_val string) error {
	return &cerrors.InvalidIgnoreFileError{ ArgVal: arg_val }
}

func (u Utils) EnvExists() error {
	var helper helpers.Helpers = helpers.Helpers{};
	env_exists, env := helper.CheckEnvExistence();

	if !env_exists {
		return &cerrors.NoEnvError{ EnvName: env };
	} else {
		return nil;
	}
}

func (u Utils) ReadIgnoresJson() types.IgnoresJsonConfig {
	var helper helpers.Helpers = helpers.Helpers{};
	ignore_json_path := helper.GetStaticFilePath(constants.IgnoreJsonFileName);
	file, err := os.Open(ignore_json_path);
	if err != nil { 
		helper.PrintInternalError();
		return nil;
	}
	defer file.Close();

	var configs types.IgnoresJsonConfig;
	decoder := json.NewDecoder(file);
	if err := decoder.Decode(&configs); err != nil { 
		helper.PrintInternalError();
		return nil;
	}

	return configs;
}

func (u Utils) CreateReadmeAndGitIgnore(ignore_file_content string) {
	var helpers helpers.Helpers = helpers.Helpers{};
	dir, _ := os.Getwd();
	dir = helpers.SanitizeDirName(dir);

	helpers.PrintInfo("Creating .gitignore file....")
	helpers.AddDelay();
	ignores_file, err := os.Create(".gitignore");
	if err != nil {
		helpers.PrintError(fmt.Sprintf("Failed to create .gitignore file in %s", dir));
		helpers.ExitGracefully();
	}
	defer ignores_file.Close();

	helpers.PrintSuccess(fmt.Sprintf(".gitignore file created in %s", dir));
	helpers.AddDelay();

	helpers.PrintInfo("Creating README.md file....")
	helpers.AddDelay();
	readme_file, err := os.Create("README.md");
	if err != nil { 
		helpers.PrintError(fmt.Sprintf("Failed to create README.md file in %s", dir));
		helpers.ExitGracefully();
	}
	defer readme_file.Close();

	helpers.PrintSuccess(fmt.Sprintf("README.md file created in %s", dir));
	helpers.AddDelay();

	if _, err := ignores_file.WriteString(ignore_file_content); err != nil {
		helpers.PrintError(fmt.Sprintf("Failed to write to .gitignore file in %s", dir));
		helpers.ExitGracefully();
	}

	if _, err := readme_file.WriteString(fmt.Sprintf("# %s\n", dir)); err != nil {
		helpers.PrintError(fmt.Sprintf("Failed to write README.md file in %s", dir));
		helpers.ExitGracefully();
	}
}

func (u Utils) ReadValidIgnoresTxt() []string {
	var helpers helpers.Helpers = helpers.Helpers{};
	valid_ignores_file_path := helpers.GetStaticFilePath(constants.ValidIgnoreListFileName);
	file, err := os.Open(valid_ignores_file_path);
	if err != nil {
		helpers.PrintInternalError();
		return nil;
	}

	var ignores []string;
	scanner := bufio.NewScanner(file);
	for scanner.Scan() {
		ignores = append(ignores, scanner.Text());
	}

	return ignores;
}

func (u Utils) CreateRemoteRepo() {
	var helpers helpers.Helpers = helpers.Helpers{};
	helpers.PrintInfo("Creating remote repository...");
	helpers.AddDelay();

	repo_name, _ := os.Getwd();
	repo_name = helpers.SanitizeDirName(repo_name);

	git_api_key := os.Getenv(constants.GitKey);
	git_user := os.Getenv(constants.GitUser);

	data := map[string]interface{} {
		"name": repo_name,
		"private": false,
		"homepage": fmt.Sprintf("%s/%s/%s", constants.GitHomePageBaseUrl, git_user, repo_name),
	}
	json_data, err := json.Marshal(data);
	if err != nil { 
		helpers.PrintInternalError();
		helpers.ExitGracefully();
	}

	req, err := http.NewRequest("POST", constants.GitApiUrl, bytes.NewBuffer(json_data));
	if err != nil { 
		helpers.PrintInternalError();
		helpers.ExitGracefully();
	}
	req.Header.Set(constants.GitContentTypeHeaderName, constants.GitContentTypeHeaderValue);
	req.Header.Set(constants.GitApiAcceptHeaderName, constants.GitApiAcceptHeaderValue);
	req.Header.Set(constants.GitApiAuthHeaderName, fmt.Sprintf("%s %s", constants.GitApiAuthHeaderValue, git_api_key));
	req.Header.Set(constants.GitHubApiVersionHeaderName, constants.GitHubApiVersionHeaderValue);

	client := &http.Client{};
	resp, err := client.Do(req);
	if err != nil { 
		helpers.PrintError(err.Error());
		helpers.ExitGracefully();
	}
	defer resp.Body.Close();

	if resp.StatusCode != 201 {
		helpers.PrintError("Failed to create remote repository");
		helpers.ExitGracefully();
	}

	helpers.PrintSuccess(fmt.Sprintf("%s created successfully", repo_name));
}

func (u Utils) RunGitCommands() {
	var helpers helpers.Helpers = helpers.Helpers{};

	dir_name, _ := os.Getwd();
	dir_name = helpers.SanitizeDirName(dir_name);
	git_user := os.Getenv(constants.GitUser);
	
	helpers.PrintInfo("Running git commands....");
	helpers.AddDelay();

	helpers.PrintInfo("Initializing the directory as a local repo....");
	helpers.AddDelay();
	git_init_cmd := exec.Command("git", "init", "-b", "main");

	_, err := git_init_cmd.Output();
	if err != nil { 
		helpers.PrintError("Failed to initialize the directory as a local repo");
		helpers.ExitGracefully();
	}

	helpers.PrintSuccess("Successfully initialized the directory as a local repo");
	helpers.AddDelay();
	helpers.PrintInfo("Adding the remote origin...")
	helpers.AddDelay();

	git_add_origin_cmd := exec.Command("git", "remote", "add", "origin", fmt.Sprintf("%s/%s/%s.git", constants.GitHomePageBaseUrl, git_user, dir_name));
	_, err = git_add_origin_cmd.Output();
	if err != nil { 
		helpers.PrintError("Failed to add the remote origin");
		helpers.ExitGracefully();
	}

	helpers.PrintSuccess("Successfully added the remote origin");
}