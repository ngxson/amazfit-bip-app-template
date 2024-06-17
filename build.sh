#!/bin/bash

set -e

PROJECT_NAME="hello_world"
LIBBIP_PATH="libbip"
LIBBIP_A_PATH="${LIBBIP_PATH}/libbip.a"
OUTPUT="build/${PROJECT_NAME}.elf"

#################################################

mkdir -p build

COMPILER_PATH="/opt/gcc-installation"
EABI="${COMPILER_PATH}/bin/arm-none-eabi"
AS="${EABI}-as"
LD="${EABI}-ld"
OBJCOPY="${EABI}-objcopy"
GCC="${EABI}-gcc"
NM="${EABI}-nm"

OBJFILES=""

for i in *.c; do
  echo "Compiling ${i%.c}.c ..."
  GCC_OPTS="-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 \
    -mfloat-abi=hard -fno-math-errno \
    -c -Os -Wa,-R -Wall -fpie -pie -fpic -mthumb -mlittle-endian -ffunction-sections -fdata-sections \
    -I ${LIBBIP_PATH} -o build/${i%.c}.o ${i%.c}.c"
  OBJFILES="${OBJFILES} build/${i%.c}.o"
  echo "$GCC $GCC_OPTS"
  $GCC $GCC_OPTS
done

echo "======================="
echo "Linking ..."
LD_OPTS="-lm -lc -EL -N -Os --cref -pie --gc-sections"
LD_OPTS="-Map build/${PROJECT_NAME}.map \
  -o $OUTPUT \
  --no-dynamic-linker \
  $LD_OPTS $OBJFILES $LIBBIP_A_PATH"
echo "$LD $LD_OPTS"
$LD $LD_OPTS

echo "======================="
echo "Add sections ..."
function add_section {
  if test -f "$2"; then
    echo "Add '$1' from '$2'"
    $OBJCOPY $OUTPUT --add-section .elf.$1=$2
  else
    echo "Skip adding '$1'"
  fi
}
add_section label label.txt
add_section name name.txt
add_section resources resources.res
add_section settings settings.bin

echo "======================="
echo "Done, output file: $OUTPUT"
