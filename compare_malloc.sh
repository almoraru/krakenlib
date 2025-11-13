# ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; #
#                                                                              #
#                        ______                                                #
#                     .-"      "-.                                             #
#                    /            \                                            #
#        _          |              |          _                                #
#       ( \         |,  .-.  .-.  ,|         / )                               #
#        > "=._     | )(__/  \__)( |     _.=" <                                #
#       (_/"=._"=._ |/     /\     \| _.="_.="\_)                               #
#              "=._ (_     ^^     _)"_.="                                      #
#                  "=\__|IIIIII|__/="                                          #
#                 _.="| \IIIIII/ |"=._                                         #
#       _     _.="_.="\          /"=._"=._     _                               #
#      ( \_.="_.="     `--------`     "=._"=._/ )                              #
#       > _.="                            "=._ <                               #
#      (_/                                    \_)                              #
#                                                                              #
#      Filename: compare_malloc.sh                                             #
#      By: espadara <espadara@pirate.capn.gg>                                  #
#      Created: 2025/11/13 22:53:32 by espadara                                #
#      Updated: 2025/11/13 22:56:38 by espadara                                #
#                                                                              #
# ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; #



#!/bin/bash

echo "🐙 Comparing Krakenlib malloc vs System malloc"
echo ""

# Test with system malloc
echo "Testing with SYSTEM malloc..."
gcc -O2 -I includes tests/malloc_bench_only.c -lm -o bench_system
./bench_system

echo ""
echo "Testing with KRAKENLIB malloc..."
gcc -O2 -I includes tests/malloc_bench_only.c krakenlib.a -lbsd -lm -o bench_kraken
./bench_kraken

rm bench_system bench_kraken
