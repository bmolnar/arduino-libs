#!/bin/bash

#set -x

###
### global settings
###
ARDUINO_DIR="/usr/share/arduino"
HARDWARE_DIRS=("${ARDUINO_DIR}/hardware")
TOOLS_DIRS=("${ARDUINO_DIR}/tools-builder" "${ARDUINO_DIR}/hardware/tools/avr")
LIBRARIES_DIRS=()

###
### user settings
###
HARDWARE_DIRS+=("${HOME}/.arduino15/packages")
TOOLS_DIRS+=("${HOME}/.arduino15/packages")

###
### project vars
###
SKETCH="${PWD}/sketches/PinTest/PinTest.ino"
FQBN="arduino:avr:uno"
#FQBN="arduino:sam:arduino_due_x_dbg"
#LIBRARIES_DIRS+=("${PWD}/libraries")
SKETCHBOOK_DIR="${PWD}"
LIBRARIES_DIRS+=("./libraries")
SKETCHES_DIR="./sketches"
BUILDPATH="${PWD}/.build"



sketches_getpath ()
{
  local sketch="${1}"; shift
  echo "${SKETCHES_DIR}/${sketch}/${sketch}.ino"
}
sketches_list ()
{
  find "${SKETCHES_DIR}" -mindepth 1 -maxdepth 1 -type d -printf "%f\n"
}
sketches_find ()
{
  local sketch="${1}"; shift
  local sketchpath=""

  sketchpath="${sketch}"
  if [ -f "${sketchpath}" ]; then
    echo "${sketchpath}"
    return 0
  fi
  sketchpath="sketches/${sketch}/${sketch}.ino"
  if [ -f "${sketchpath}" ]; then
    echo "${sketchpath}"
    return 0
  fi
  return 1
}




cmd_build ()
{
  local sketchpath="${SKETCH}"
  if [ "${#}" -ge 1 ]; then
    sketch="${1}"; shift
    sketchpath="$(sketches_find "${sketch}")"
  fi

  if ! [ -f "${sketchpath}" ]; then
    echo "Invalid sketch \"${sketch}\"" 1>&2
    return 1
  fi

  mkdir -p "${BUILDPATH}"

  local args=()
  args+=("-logger=human")
  for idx in "${HARDWARE_DIRS[@]}"; do
    args+=("-hardware=${idx}")
  done
  for idx in "${TOOLS_DIRS[@]}"; do
    args+=("-tools=${idx}")
  done
  args+=("-built-in-libraries=${ARDUINO_DIR}/libraries")
  for idx in "${LIBRARIES_DIRS[@]}"; do
    args+=("-libraries=${idx}")
  done
  args+=("-fqbn=${FQBN}")
  args+=("-ide-version=10607")
  args+=("-build-path=${BUILDPATH}")
  args+=("-warnings=none")
  args+=("-prefs=build.warn_data_percentage=75")
  args+=("-verbose")

  #"${ARDUINO_DIR}/arduino-builder" -dump-prefs "${args[@]}" "${SKETCH}"
  "${ARDUINO_DIR}/arduino-builder" -compile "${args[@]}" "${sketchpath}"
}
cmd_clean ()
{
  rm -rf "${BUILDPATH}"
}
cmd_help ()
{
    echo "Usage:"
    echo "    helper.sh <command> [<args>]"
    echo ""
    echo "  Commands:"
    echo "    build     Compile and verify sketch"
    echo "    clean     Clean local build artifacts"
    echo "    help      Displays this information"
    echo "    list      List sketches"
    echo "    upload    Upload sketch to device"
    echo ""
}
cmd_list ()
{
  sketches_list
}
cmd_upload ()
{
  echo
}



cmd="${1}"; shift
case "${cmd}" in
  build)    cmd_build  "${@}";;
  clean)    cmd_clean  "${@}";;
  ""|help)  cmd_help   "${@}";;
  list)     cmd_list   "${@}";;
  upload)   cmd_upload "${@}";;
  *)        echo "helper.sh: '${cmd}' is not a valid command. Please use 'helper.sh help'." 1>&2;;
esac
