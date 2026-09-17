#! /bin/bash

ScriptPath=$0
Dir=$(cd $(dirname "$ScriptPath"); pwd)
Basename=$(basename "$ScriptPath")
CMakeDir=${SIS_CMAKE_BUILD_DIR:-$Dir/_build}
[[ -n "$MSYSTEM" ]] && DefaultMakeCmd=mingw32-make.exe || DefaultMakeCmd=make
MakeCmd=${SIS_CMAKE_MAKE_COMMAND:-${SIS_CMAKE_COMMAND:-$DefaultMakeCmd}}
ProjectNameFile="$Dir/.sis/project_name.txt"
ProjectName=$(tr -d '[:space:]' < "$ProjectNameFile")

IgnoreRemainingFlagsAndOptions=0
Targets=()


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
# functions

function join_by { local IFS="$1"; shift; echo "$*"; }


# ##########################################################
# command-line handling

while [[ $# -gt 0 ]]; do

  if [ $IgnoreRemainingFlagsAndOptions -ne 0 ]; then

    Targets+=($1)

    shift

    continue
  else

    if [ ! ${1:0:1} = '-' ]; then

      Targets+=($1)

      shift

      continue
    fi
  fi

  case $1 in
    --)

      IgnoreRemainingFlagsAndOptions=1
      ;;
    --help)

      [ -f "$Dir/.sis/script_info_lines.txt" ] && cat "$Dir/.sis/script_info_lines.txt"
      cat << EOF
Executes CMake-generated artefacts to (re)build project

$ScriptPath [ ... flags/options ... ]

Flags/options:

    behaviour:


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

if [ ! -d "$CMakeDir" ]; then

  >&2 echo "$ScriptPath: CMake build directory '$CMakeDir' not found so nothing to do; use script 'prepare_cmake.sh' if you wish to prepare CMake artefacts"

  exit 1
else

  cd $CMakeDir

  if [ ! -f "$CMakeDir/Makefile" ]; then

    >&2 echo "$ScriptPath: CMake build directory '$CMakeDir' does not contain expected file 'Makefile', so a clean cannot be performed. It is recommended that you remove all CMake artefacts using script 'remove_cmake_artefacts.sh' followed by regeneration via 'prepare_cmake.sh'"

    cd ->/dev/null

    exit 1
  else

    if [ -z "$Targets" ]; then

      echo "Executing build for ${ProjectNameClr} (via command \`${MakeCmdClr}\`)"
    else

      echo "Executing build for ${ProjectNameClr} (via command \`${MakeCmdClr}\`) with specific target(s) $(join_by , "${Targets[@]}")"
    fi

    $MakeCmd ${Targets[*]}
    status=$?

    cd ->/dev/null

    exit $status
  fi
fi


# ############################## end of file ############################# #

