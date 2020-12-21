#include "UserParams.h"


UserParams::UserParams() {
    ArgMapping amap;
    amap.arg("dry", dry, "dry=1 for a dry-run");
    amap.arg("R", R, "number of rounds");
    amap.arg("p", p, "plaintext base");
    amap.arg("r", r,  "lifting");
    amap.arg("d", d, "degree of the field extension");
    amap.note("d == 0 => factors[0] defines extension");
    amap.arg("c", c, "number of columns in the key-switching matrices");
    amap.arg("k", k, "security parameter");
    amap.arg("L", L, "# of levels in the modulus chain",  "heuristic");
    amap.arg("s", s, "minimum number of slots");
    amap.arg("repeat", repeat,  "number of times to repeat the test");
    amap.arg("m", chosen_m, "use specified value as modulus", NULL);
    amap.arg("mvec", mvec, "use product of the integers as  modulus", NULL);
    amap.note("e.g., mvec='[5 3 187]' (this overwrite the m argument)");
    amap.arg("gens", gens, "use specified vector of generators", NULL);
    amap.note("e.g., gens='[562 1871 751]'");
    amap.arg("ords", ords, "use specified vector of orders", NULL);
    amap.note("e.g., ords='[4 2 -4]', negative means 'bad'");
    amap.arg("seed", seed, "PRG seed");
    amap.arg("nt", nt, "num threads");
    amap.arg("noPrint", noPrint, "suppress printouts");
}

void UserParams::parse(int argc, char** argv) {
    amap.parse(argc, argv);
}