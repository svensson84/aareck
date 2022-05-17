#!/usr/bin/env bash

function showUsage() {
    read -r -d '' usage <<-EOF

    Usage: publish OPTIONS
        Publish this node package to the configured environment <.npmrc>.

        Options:
        -f, --file=FILE     read environmental variables for <.npmrc> from FILE
        
EOF
    echo "$usage"
}

function showLocalNpmrcVars() {
    grep -oP '(?<=\${).*?(?=})' ./npm/.npmrc | 
        sed s/^/'$'/ | 
        xargs -L 1 -I {} echo "echo '\{}'=`echo {}`" | 
        xargs -L 1 -I {} sh -c "{}"
}

function exportLocalNpmrcVars() {
    set -a
    source $1
    set +a
}

function doPublishPackage() {
    echo "> following values are applied for <.npmrc>"
    showLocalNpmrcVars
    npm config set registry $REGISTRY_URL
    echo -e "\ncheck npm configuration entries:"
    npm config list
    npm publish
}

if [ -z "$1" ]; then
    echo -e "ERROR: no option supplied!\n"
    showUsage
    exit 1
elif [ "$1" == "-f" ] || [ "$1" == "--file" ]; then
    if [ -z "$2" ]; then
        echo -e "ERROR: no environment FILE supplied!\n"
        showUsage
        exit 1
    fi
    echo -e "environmental variables in file <$2> are exported\n"
    exportLocalNpmrcVars "$2"
    doPublishPackage
else
    echo -e "ERROR: invalid args\n"
    showUsage
    exit 1
fi