#include <iostream>
#include <cmath>


int main() {
    // 長打率と打数を入力
    double slugging_percentage = 0.539;
    int at_bats = 133;

    // 長打率から合計ベース数を計算
    int total_bases = static_cast<int>(std::round(slugging_percentage * at_bats));

    // 単打、二塁打、三塁打、本塁打の数を推定
    int singles, doubles, triples, home_runs;
    // 三塁打を極端に少なくするための割合を設定
    const double TRIPLE_RATIO = 0.05; // 三塁打の割合を全体の5%に設定

    // 本塁打と二塁打は三塁打の10倍と仮定
    home_runs = static_cast<int>(total_bases * TRIPLE_RATIO);
    doubles = home_runs * 10;

    // 三塁打は非常に珍しいので、1または0に設定
    triples = (total_bases > 100) ? 1 : 0; // 打数が100以上の場合は1、それ以下は0

    // 残りのベース数を単打として計算
    singles = total_bases - (2 * doubles + 3 * triples + 4 * home_runs);

    // 合計ベース数が正確になるように調整
    while (singles + 2 * doubles + 3 * triples + 4 * home_runs < total_bases) {
        singles++;
    }

    // 結果を出力
    std::cout << "単打: " << singles << std::endl;
    std::cout << "二塁打: " << doubles << std::endl;
    std::cout << "三塁打: " << triples << std::endl;
    std::cout << "本塁打: " << home_runs << std::endl;

    return 0;
}

