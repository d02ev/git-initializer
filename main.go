package main

import (
	"fmt"
	"os"
	"strings"

	"github.com/d02ev/git-initializer/constants"
	"github.com/d02ev/git-initializer/helpers"
	"github.com/d02ev/git-initializer/parser"
	"github.com/d02ev/git-initializer/utils"
)

func main() {
	var args []string = os.Args;
	var arg_count int = len(args);
	var utils utils.Utils = utils.Utils{};
	var arg_parser parsers.ArgParser = parsers.ArgParser{};
	var helpers helpers.Helpers = helpers.Helpers{};

	if arg_count == 1 {
		helpers.PrintInfo(constants.HelpMessage);
		return;
	}

	count_err := utils.HasCorrectArgCount(args[1], arg_count);
	if count_err != nil {
		fmt.Println(count_err.Error());
		return;
	}
	
	switch args[1] {
		case constants.InitArg:
			if arg_count == 2 {
				arg_parser.ParseInitArg("");
				return;
			}

			ignore_file_split := strings.Split(args[2], "=");
			arg_parser.ParseInitArg(ignore_file_split[1]);
 		
		case constants.AddArg:
			// parse the add arg
			key_split := strings.Split(args[2], "=");
			user_split := strings.Split(args[3], "=");
			// git-initializer add --key="GIT_API_KEY" --user="GIT_USER"
			arg_parser.ParseAddArg(key_split[1], user_split[1]);
		case constants.ListAvailableCmdsArg:
			// parse the lsc arg
			break;
		case constants.ListIgnoresArg:
			// parse the lsig arg
			arg_parser.ParseListValidIgnoreFileArg();
		default:
			invalid_arg_name_err := utils.InvalidArgName(args[1]);
			if invalid_arg_name_err != nil {
				fmt.Println(invalid_arg_name_err.Error());
				return;
			}
	}
}