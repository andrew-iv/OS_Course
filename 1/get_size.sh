xwininfo -root| awk '/^.*-geometry.*/ {
match($2, "^[0-9]+x");
print(substr($2, RSTART, RLENGTH-1));
match($2, "x[0-9]+\\+");
print(substr($2, RSTART+1, RLENGTH-2)) }'
