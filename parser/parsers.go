package parsers

import (
	"fmt"
	"os"
	"os/user"
	"path/filepath"
	"strings"
	"github.com/d02ev/git-initializer/constants"
	"github.com/d02ev/git-initializer/helpers"
	"github.com/d02ev/git-initializer/utils"
)

type ArgParser struct {}

func (a ArgParser) ParseAddArg(git_key string, git_user string) {
	var utils utils.Utils = utils.Utils{};
	var helpers helpers.Helpers = helpers.Helpers{};

	// check if the values supplied are not empty
	empty_key_val_err := utils.HasEmptyArgValue(constants.GitKey, git_key);
	empty_user_val_err := utils.HasEmptyArgValue(constants.GitUser, git_user);
	if empty_key_val_err != nil {
		helpers.PrintError(empty_key_val_err.Error());
		helpers.ExitGracefully();
	}
	if empty_user_val_err != nil {
		helpers.PrintError(empty_user_val_err.Error());
		helpers.ExitGracefully();
	}

	helpers.PrintInfo(fmt.Sprintf("Checking if %s and %s are already added to the user profile....", constants.GitKey, constants.GitUser));
	helpers.AddDelay();

	usr, err := user.Current();
	if err != nil { 
		helpers.PrintInternalError();
		helpers.ExitGracefully();
	}
	profile := filepath.Join(usr.HomeDir, ".bashrc");
	file, err := os.OpenFile(profile, os.O_APPEND | os.O_WRONLY, 0666);
	if err != nil { 
		helpers.PrintInternalError();
		helpers.ExitGracefully();
	}
	defer file.Close();

	env_exists, env := helpers.CheckEnvExistence();

	if env_exists {
		helpers.PrintInfo(fmt.Sprintf("%s already added to the user profile", env));
		helpers.ExitGracefully();
	}

	helpers.PrintInfo(fmt.Sprintf("Adding %s and %s to the user profile....", constants.GitKey, constants.GitUser));
	helpers.AddDelay();
	key_env_line := fmt.Sprintf("export %s=%s", constants.GitKey, git_key);
	user_env_line := fmt.Sprintf("export %s=%s", constants.GitUser, git_user);
	helpers.AddDelay();

	if _, err := file.WriteString("\n" + key_env_line + "\n" + user_env_line + "\n"); err != nil {
		helpers.PrintInternalError();
		helpers.ExitGracefully();
	}

	helpers.PrintSuccess(fmt.Sprintf("%s and %s successfully added to the user profile", constants.GitKey, constants.GitUser));
	helpers.PrintInfo("To apply changes immediately, run: ");
 	helpers.PrintInfo(fmt.Sprintf("source %s", profile));
}

func (a ArgParser) ParseInitArg(ignore_arg_value string) {
	var utils utils.Utils = utils.Utils{};
	var helpers helpers.Helpers = helpers.Helpers{};

	helpers.PrintInfo(fmt.Sprintf("Checking if %s and %s are already added to the user profile....", constants.GitKey, constants.GitUser));
	helpers.AddDelay();

	env_non_existence_err := utils.EnvExists();
	if env_non_existence_err != nil {
		helpers.PrintError(env_non_existence_err.Error());
		helpers.PrintInfo("Please run: git-initializer add --key=<key> --user=<user> to add <key> and <user> before initializing a new repository");
		helpers.ExitGracefully();
	}

	var ignore_files = "";
	if ignore_arg_value != "" {
		ignore_files = strings.TrimSpace(ignore_arg_value);
	} else {
		helpers.PrintInfo(fmt.Sprintf("No ignore files specified - defaulting to: %s", constants.DefaultIgnoreFiles));
		ignore_files = constants.DefaultIgnoreFiles;
	}

	var ignore_files_split = strings.Split(ignore_files, ",");
	var ignore_file_content string = "";
	config := utils.ReadIgnoresJson();

	for ig := range ignore_files_split {
		if _, ok := config[ignore_files_split[ig]]; !ok {
			invalid_ignore_file_err := utils.InvalidIgnoreFileValue(ignore_files_split[ig]);
			if invalid_ignore_file_err != nil {
				helpers.PrintError(invalid_ignore_file_err.Error());
				helpers.PrintInfo("Please run: git-initializer lsig to see available ignore files");
				helpers.ExitGracefully();
			}
		}
		ignore_file_content += config[ignore_files_split[ig]].Contents;
	}

	utils.CreateReadmeAndGitIgnore(ignore_file_content);
	helpers.AddDelay();
	
	utils.CreateRemoteRepo();
	helpers.AddDelay();
	
	helpers.PrintInfo("Initializing the local repository....");
	helpers.AddDelay();

	utils.RunGitCommands();
	helpers.AddDelay();

	helpers.PrintSuccess("Done!");
}

func (a ArgParser) ParseListValidIgnoreFileArg() {
	var utils utils.Utils = utils.Utils{};
	var helpers helpers.Helpers = helpers.Helpers{};
	
	helpers.PrintInfo("Here's a list of valid ignore files: ");

	ignores := utils.ReadValidIgnoresTxt();
	for ig := range ignores {
		fmt.Println(ignores[ig]);
	}
}