valgrind --show-leak-kinds=definite,indirect --leak-check=full --track-origins=yes --log-file=valgrind.out ./compiler $1
