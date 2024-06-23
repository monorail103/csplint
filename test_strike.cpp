#include <cmath>
#include <iostream>

/*
Copilot
ストライクが入る確率を計算するためには、投手のデータから関連する統計を使用して数学的なモデルを構築する必要があります。以下に、簡単なモデルを示します。このモデルでは、防御率(ERA)、奪三振(SO)、与四球(BB)、最高球速(Vmax)を考慮に入れています。

ストライクが入る確率 ( P(S) ) を次のように定義します：

P(S)=1/(1+e−(a⋅ERA+b⋅SO+c⋅BB+d⋅Vmax))

ここで、( e ) は自然対数の底、( a ), ( b ), ( c ), ( d ) はモデルのパラメータです。これらのパラメータは、実際のデータを用いて機械学習などの方法で最適化することができます。

例えば、防御率が低いほど、奪三振が多いほど、与四球が少ないほど、最高球速が速いほどストライクが入る確率が高くなると仮定すると、( a ) は負の値、( b ) と ( d ) は正の値、( c ) は負の値を取ることになります。

この式はロジスティック回帰モデルの一種であり、実際の野球のデータに基づいてパラメータを調整することで、より現実に近い確率を計算することができます。ただし、これはあくまで一例であり、実際にはもっと複雑なモデルが必要になるかもしれません。また、データの収集と分析が重要になります。
*/

// ロジスティック回帰モデルのパラメータ（実際のデータに基づいて調整する）
const double a = -0.1; // ERAに対する係数
const double b = 0.05; // SOに対する係数
const double c = -0.05; // BBに対する係数
const double d = 0.01; // Vmaxに対する係数

// ストライクが入る確率 P(S) を計算する関数
double calculateStrikeProbability(double ERA, int SO, int BB, double Vmax) {
    double exponent = a * ERA + b * SO + c * BB + d * Vmax;
    double probability = 1 / (1 + exp(-exponent));
    return probability;
}

int main() {
    // テストデータ
    double ERA = 2.85; // 防御率
    int SO = 80; // 奪三振
    int BB = 20; // 与四球
    double Vmax = 150.0; // 最高球速 (km/h)

    // ストライクが入る確率を計算
    double strikeProbability = calculateStrikeProbability(ERA, SO, BB, Vmax);

    // 結果を表示
    std::cout << "ストライクが入る確率: " << strikeProbability << std::endl;

    return 0;
}