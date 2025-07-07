for i in {1..20}; do
    ./ag -c 2 -p 1 -i 5 >> output.txt
    echo "\n\n" >> output.txt
done;