#!/bin/bash
# Automatic build script
# Uses zed and zip command

build_dir="./build"
project="mpdq"
version_file="./src/util.c"
bundled_files="README.md LICENSE"

function build_failed
{
	echo "Make returned errors, aborting..."
	sed -i -e 's/$version/@VERSION@/g' "$version_file"
	exit
}

if test -d $build_dir; then
	echo "Build directory exists..."
else
	echo "Creating build directory..."
	mkdir ./$build_dir
fi

version=$1

if [ -z "$version" ]; then
	echo "Please provide a version string"
	exit
fi

echo "Building $project $version..."

if [[ -z "$version_file" ]]; then
	echo "No version file specified! Skipping insertion of build number..."
else
	echo "Writing build number to $version_file..."
	sed -i -e "s/@VERSION@/$version/g" $version_file
fi

echo "Running make..."
make || build_failed

echo "Build done"

echo "Resetting build version..."
sed -i -e "s/$version/@VERSION@/g" $version_file

echo "Zipping build to $build_dir/$project-$version.zip..."
zip "$build_dir/$project-$version.zip" $project $bundled_files
echo "Build successful!"