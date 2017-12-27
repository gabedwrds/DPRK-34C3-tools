echo 
echo "$@"
./decrypt "$@" > "$@.dec"
if [ -s  "$@.dec" ]; then
	echo "decrypt OK, moving"
	mv "$@.dec" "$@"
else
	echo "decrypt failed, keeping"
	rm "$@.dec"
fi
./trim_pdf "$@"