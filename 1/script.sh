#set -e
FR=10
D=0
S=0

X=0
Y=0
Flag=1
hostname=localhost
username=andrew

[ -z $T1_FR ] || FR=$T1_FR
[ -z $T1_D ] || D=$T1_D
[ -z $T1_S ] || S=$T1_S

[ -z $T1_hostname ] || hostname=$T1_hostname
[ -z $T1_username ] || Y=$T1_username
[ -z $T1_X ] || X=$T1_X
[ -z $T1_Y ] || Y=$T1_Y
 
if [ $T1_X ] && [ $T1_Y ] ;then Flag=0;fi

SS=$X"x"$Y
cmd=$(cat <<EOSSH
if [[ $Flag -eq 0 ]] ; then
#echo aa-$size
ffmpeg -f x11grab -r $FR -s $SS -i ":"$D"."$S -f avi pipe:1
else
size=$(xwininfo -root| awk '/^.*-geometry.*/ {match($2, "^[0-9]+x[0-9]+\s+");print(substr($2, RSTART, RLENGTH-1));}')
ffmpeg -f x11grab -r $FR -s 1024x600 -i ":"$D"."$S -f avi  pipe:1
fi
#ffmpeg -f oss -i /dev/dsp -f video4linux2 -r 20 -s $size -i /dev/video0 -f avi pipe:1
EOSSH
)
mkfifo /tmp/fifo1
ssh $username@$hostname "$cmd" >> /tmp/fifo1 &
ffplay -f avi /tmp/fifo1



