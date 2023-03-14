N=10

for seed in {1..$N}
do
    python genInput.py 3 2 input_size3.txt $seed
    ./build/puzzle_solver -n 3 -i ./res/input_size3.txt -a AStar -k 2
done

# for seed in {1..$N}
# do
#     python genInput.py 4 2 input_size4.txt $seed
#     ./main input_size4.txt 4
# done

# for seed in {1..$N}
# do
#     python genInput.py 5 2 input_size5.txt $seed
#     ./main input_size5.txt 5
# done

# for seed in {1..$N}
# do
#     python genInput.py 6 2 input_size6.txt $seed
#     ./main input_size6.txt 6
# done