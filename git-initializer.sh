#!/bin/bash

# variables
REPO_URL="https://github.com/d02ev/git-initializer.git"
INSTALL_DIR="/usr/local/bin"
TOOL_NAME="git-initializer"
CONFIG_DIR="$HOME/.git-initializer"

# check if Go is installed
if ! command -v go &> /dev/null; then
  echo "The tool requires Go to be installed. Please install Go and try again."
  echo "Install Go from https://golang.org/dl/"
  exit 1

# clone the repository
echo "Cloning the repository...."
git clone "$REPO_URL" "$TOOL_NAME-temp"
cd "$TOOL_NAME-temp" || exit

# Build the tool
echo "Building $TOOL_NAME...."
go build -o "$TOOL_NAME"

# Move the binary to the installation dir
echo "Installing $TOOL_NAME to $INSTALL_DIR...."
sudo mv "$TOOL_NAME" "$INSTALL_DIR"

# Create config dir for static files
echo "Creating config dir at $CONFIG_DIR...."
mkdir -p "$CONFIG_DIR"

# Copy static files to config dir
echo "Copying static files to $CONFIG_DIR...."
cp -r static/* "$CONFIG_DIR/"

# Cleanup
echo "Cleaning up...."
cd .. || exit
rm -rf "$TOOL_NAME-temp"

# Success
echo "$TOOL_NAME installed successfully!"
echo "Run '$TOOL_NAME' in the terminal to get started."

echo "To configure your Git credentials, use:"
echo "$TOOL_NAME add --key=YOUR_GIT_API_KEY --user=YOUR_GIT_USERNAME"