#!bin/bash

pwd=`pwd`

echo "Downloading files:"

[ -d 42cursus_gnl_tests ] || git clone https://github.com/mrjvs/42cursus_gnl_te\
sts.git

[ -d get-next-line-testing-tools ] || git clone https://github.com/TinfoilPanca\
kes/get-next-line-testing-tools.git

[ -d 42TESTERS-GNL ] || git clone https://github.com/Mazoise/42TESTERS-GNL.git

[ -d gnlTester ] || git clone https://github.com/Tripouille/gnlTester.git

echo "Download complete."
# echo "Running tests:"

cd $pwd/42cursus_gnl_tests
if [ ! -f ".altered" ]; then
	sed -i 's/.\/get_next_line/../'  Makefile
	sed -i '12 s/\.\//bash &/' Makefile
	sed -i '16 s/\.\//bash &/' Makefile
	sed -i '52 s/\.\//bash &/' compile_and_run.sh
	sed -i '60 s/\.\//bash &/' compile_and_run.sh
	touch .altered
fi
make
sleep 2
make bonus

echo -e '\u001b[0m'
cd $pwd/get-next-line-testing-tools
if [ ! -f ".altered" ]; then
	sed -i 's/GNL_SOURCE_DIR = ..\/get_next_line/GNL_SOURCE_DIR = ../' Makefile

	sed -i 's/int\tdiff_file_size;/int\tdiff_file_size;\n\tchar\tt[11];/' mouli\
test_files/get_next_line_tests/tests/41_hard_test_large_file.spec.c

	sed -i 's/read(fd3, NULL, 10);/read(fd3, \&t, 10);/' moulitest_files/get_ne\
xt_line_tests/tests/41_hard_test_large_file.spec.c

	sed -i 's/int\tdiff_file_size;/int\tdiff_file_size;\n\tchar\tt[11];/' mouli\
test_files/get_next_line_tests/tests/42_hard_test_one_big_fat_line.spec.c

	sed -i 's/read(fd3, NULL, 10);/read(fd3, \&t, 10);/' moulitest_files/get_ne\
xt_line_tests/tests/42_hard_test_one_big_fat_line.spec.c

	sed -i 's/..\/..\/..\/..\/get_next_line/..\/..\/..\/../' moulitest_files/te\
stframework/v3/Makefile

	touch .altered
fi
make mtest_all

echo -e '\u001b[0m'

cd $pwd/42TESTERS-GNL
bash ./all_tests_with_bonus.sh

echo -e '\u001b[0m'

cd $pwd/gnlTester
make a

echo -e '\033[0m'
 