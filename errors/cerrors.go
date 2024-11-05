package cerrors

import (
	"fmt"
)

type InvalidArgCountError struct {
	CountDirection int;
	ArgName string;
}
type InvalidArgError struct {
	ArgName string;
}
type InvalidFlagError struct {
	FlagName string;
}
type InvalidIgnoreFileError struct {
	ArgVal string;
}
type EmptyArgValueError struct {
	ArgName string;
}
type NoEnvError struct {
	EnvName string;
}


func (e *InvalidArgCountError) Error() string {
	if (e.CountDirection == 1) {
		return fmt.Sprintf("Too many arguments for \"%s\"", e.ArgName);
	}
	if (e.CountDirection == -1) {
		return fmt.Sprintf("Too few arguments for \"%s\"", e.ArgName);
	}

	return fmt.Sprintf("Invalid argument count for \"%s\"", e.ArgName);
}

func (e *EmptyArgValueError) Error() string {
	return fmt.Sprintf("%s cannot have empty value", e.ArgName);
}

func (e *NoEnvError) Error() string { 
	return fmt.Sprintf("%s not set", e.EnvName);
}

func (e *InvalidArgError) Error() string { 
	return fmt.Sprintf("Invalid argument: %s", e.ArgName);
}

func (e *InvalidIgnoreFileError) Error() string { 
	return fmt.Sprintf("Invalid ignore file: %s", e.ArgVal);
}

func (e *InvalidFlagError) Error() string { 
	return fmt.Sprintf("Invalid flag: %s", e.FlagName);
}