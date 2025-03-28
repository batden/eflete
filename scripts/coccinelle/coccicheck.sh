#!/bin/sh
set -eu

read -p "Type the path to the testing folder [src/bin]: " path
if [ -z $path ]; then
   path='src/bin'
else
   path=$path
fi

COCCI_FILES="
andconst.cocci
badzero.cocci
continue.cocci
free_stack.cocci
mutex.cocci
notand.cocci
notnull.cocci
null_ref.cocci
unused.cocci
use_after_iter.cocci
macros.cocci
"

HAS_ERROR=0
for f in $COCCI_FILES; do
   OPTIONS=""
   if [ "$f" = "macros.cocci" ]; then
      OPTIONS="--defined DIV_ROUND_UP --defined ROUND_UP --defined MIN --defined MAX"
   fi
   CMD="spatch --timeout 200 --very-quiet --cocci-file scripts/coccinelle/$f --include-headers --dir $path $OPTIONS"
   OUT=$($CMD)
   echo "$CMD"
   if [ -n "$OUT" ]; then
      echo "$OUT"
      HAS_ERROR=1
   fi
done
exit $HAS_ERROR
