package helpers

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"
	"time"
	"github.com/d02ev/git-initializer/constants"
)

type Helpers struct {}

func (h Helpers) CheckEnvExistence() (bool, string) {
	git_key_env := os.Getenv(constants.GitKey);
	git_user_env := os.Getenv(constants.GitUser);

	if git_key_env == "" && git_user_env == "" {
		return false, fmt.Sprintf("%s and %s", constants.GitKey, constants.GitUser);
	}
	if git_key_env == "" {
		return false, constants.GitKey;
	}
	if git_user_env == "" {
		return false, constants.GitUser;
	}

	return true, "";
}

func (h Helpers) SanitizeDirName(dir_name string) string {
	sanitized_dir_name_split := strings.Split(dir_name, "/");
	dir_name = sanitized_dir_name_split[len(sanitized_dir_name_split) - 1];

	return dir_name
}

func (h Helpers) PrintError(err_message string) {
	fmt.Println(constants.ErrorColor + err_message + constants.Reset);
}

func (h Helpers) PrintInfo(info_message string) {
	fmt.Println(constants.InfoColor + info_message + constants.Reset);
}

func (h Helpers) PrintInternalError() {
	fmt.Println(constants.ErrorColor + "An internal error occurred. Please try again later" + constants.Reset);
}

func (h Helpers) PrintSuccess(success_message string) {
	fmt.Println(constants.SuccessColor + success_message + constants.Reset);
}

func (h Helpers) AddDelay() {
	time.Sleep(1 * time.Second);
}

func (h Helpers) GetStaticFilePath(filename string) string {
	home_dir, err := os.UserHomeDir();
	if err != nil {
		h.PrintInternalError();
		return "";
	}
	return filepath.Join(home_dir, constants.StaticFileDirName, filename);
}

func (h Helpers) ExitGracefully() {
	h.PrintInfo("Attempting to exit gracefully....");
	h.AddDelay();
	os.Exit(1);
}