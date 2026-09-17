#! /bin/bash

ScriptPath=$0
Dir=$(cd $(dirname "$ScriptPath"); pwd)
Basename=$(basename "$ScriptPath")
CMakeDir=${SIS_CMAKE_BUILD_DIR:-$Dir/_build}
[[ -n "$MSYSTEM" ]] && DefaultMakeCmd=mingw32-make.exe || DefaultMakeCmd=make
MakeCmd=${SIS_CMAKE_MAKE_COMMAND:-${SIS_CMAKE_COMMAND:-$DefaultMakeCmd}}
ProjectNameFile="$Dir/.sis/project_name.txt"
ProjectName=$(tr -d '[:space:]' < "$ProjectNameFile")

ListOnly=0
RunMake=1


# ##########################################################
# colours

if [ -n "${TERM:-}" ] && [ -t 1 ] && command -v tput >/dev/null 2>&1; then

  RbEnvClr_Blue=${FG_BLUE:-$(tput setaf 4)}
  RbEnvClr_Red=${FG_BLUE:-$(tput setaf 1)}
  RbEnvClr_Bold=${FD_BOLD:-$(tput bold)}
  RbEnvClr_None=${FD_NONE:-$(tput sgr0)}
else

  RbEnvClr_Blue=
  RbEnvClr_Red=
  RbEnvClr_Bold=
  RbEnvClr_None=
fi

MakeCmdClr="${RbEnvClr_Blue}${RbEnvClr_Bold}${MakeCmd}${RbEnvClr_None}"
ProjectNameClr="${RbEnvClr_Blue}${RbEnvClr_Bold}${ProjectName}${RbEnvClr_None}"


# ##########################################################
# command-line handling

while [[ $# -gt 0 ]]; do

  case $1 in
    --list-only|-l)

      ListOnly=1
      ;;
    --no-make|-M)

      RunMake=0
      ;;
    --help)

      [ -f "$Dir/.sis/script_info_lines.txt" ] && cat "$Dir/.sis/script_info_lines.txt"
      cat << EOF
Runs all example programs

$ScriptPath [ ... flags/options ... ]

Flags/options:

    behaviour:

    -l
    --list-only
        lists the target programs but does not execute them

    -M
    --no-make
        does not execute CMake and make before running tests


    standard flags:

    --help
        displays this help and terminates

EOF

      exit 0
      ;;
    *)

      >&2 echo "$ScriptPath: unrecognised argument '${RbEnvClr_Red}${RbEnvClr_Bold}$1${RbEnvClr_None}'; use --help for usage"

      exit 1
      ;;
  esac

  shift
done


# ##########################################################
# main()

status=0

if [ $RunMake -ne 0 ]; then

  if [ $ListOnly -eq 0 ]; then

    echo "Executing build (via command \`${MakeCmdClr}\`) and then running all ${ProjectNameClr} example programs"

    mkdir -p $CMakeDir || exit 1

    cd $CMakeDir

    $MakeCmd
    status=$?

    cd ->/dev/null
  fi
else

  if [ ! -d "$CMakeDir" ] || [ ! -f "$CMakeDir/CMakeCache.txt" ] || [ ! -d "$CMakeDir/CMakeFiles" ]; then

    >&2 echo "$ScriptPath: cannot run in '--no-make' mode without a previous successful build step"
  fi
fi

if [ $status -eq 0 ]; then

  if [ $ListOnly -ne 0 ]; then

    echo "Listing all ${ProjectNameClr} example programs"
  else

    echo "Running all ${ProjectNameClr} example programs"
  fi

  for f in $(find $CMakeDir/examples -type f -exec test -x {} \; -print)
  do

    if [ $ListOnly -ne 0 ]; then

      echo "would execute ${RbEnvClr_Blue}${RbEnvClr_Bold}${f}${RbEnvClr_None}:"

      continue
    fi

    echo
    echo "executing ${RbEnvClr_Blue}${RbEnvClr_Bold}${f}${RbEnvClr_None}:"

    $f
  done
fi

exit $status


# ############################## end of file ############################# #

