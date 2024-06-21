#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib> // std::rand, std::srand
#include <ctime>   // std::time
#include <random> // Add this line to include the <random> header
using namespace std;

/*
0: アウト
1: 単打
2: 二塁打
3: 三塁打
4: ホームラン
*/

// 構造体の定義
struct PlayerStats {
    std::string name;
    // 背番号
    int num;
    // 打率(AVG), 長打率(SLG), 打数(AB), 安打数(H), 盗塁数(SB), 出塁率(OBP)
    double avg;
    double slg;
    int ab;
    int h;
    int sb;
    double obp;
};

// 打席結果を文字列で返す関数
string getAtBatResultString(int result) {
    switch (result) {
        case 0: return "四球";
        case 1: return "単打";
        case 2: return "二塁打";
        case 3: return "三塁打";
        case 4: return "ホームラン";
        case 5: return "フライ";
        case 6: return "ゴロ";
        case 7: return "三振";
        default: return "不明な結果";
    }
}

// ベースとスコアの状態を管理する変数
std::string one_base = "なし", two_base = "なし", thr_base = "なし";
int score = 0, base = 0;

// ベースを進める関数
void advanceBases(std::string &runner, int &score, bool scoreIfOccupied) {
    if(runner != "なし") {
        if(scoreIfOccupied) score++;
        runner = "なし";
    }
}

// ランナーを押し出す関数
void pushRunner(std::string &fromBase, std::string &toBase, int &score) {
    if(fromBase != "なし") {
        if(toBase != "なし") {
            // 次の塁にランナーがいる場合、スコアを増やす
            score++;
        } else {
            // 次の塁にランナーがいない場合、ランナーを移動させる
            toBase = fromBase;
        }
        fromBase = "なし";
    }
}

// 打席結果に応じてベースを進める関数
void hitResult(int hitType, std::vector<PlayerStats> &players, int &now_batter) {
    if(hitType >= 5) return; // アウトの場合、何もしない

    // ホームランの場合、全てのランナーがスコアする
    else if(hitType == 4) {
        advanceBases(one_base, score, true);
        advanceBases(two_base, score, true);
        advanceBases(thr_base, score, true);
        score++; // バッターもスコアする
        base = 0; // ベースをクリア
    } else {
        // ベースを進める
        if(hitType >= 0) pushRunner(two_base, thr_base, score);
        if(hitType >= 2) pushRunner(one_base, two_base, score);

        // ヒットに応じてランナーを配置
        switch(hitType) {
            case 0: 
                // 1から2にランナーを進める
                pushRunner(one_base, two_base, score);
                // 3塁ランナーをホームイン
                advanceBases(thr_base, score, true);
                one_base = players[now_batter].name; 
                break;
            case 1: 
                // 1から2にランナーを進める
                pushRunner(one_base, two_base, score);
                // 3塁ランナーをホームイン
                advanceBases(thr_base, score, true);
                one_base = players[now_batter].name; 
                break;
            case 2: 
                // 1から3にランナーを進める
                pushRunner(one_base, thr_base, score);
                // 2,3塁ランナーをホームイン
                advanceBases(two_base, score, true);
                advanceBases(thr_base, score, true);
                two_base = players[now_batter].name; 
                break;
            case 3:
                // 1,2,3からホームにランナーを進める
                advanceBases(one_base, score, true);
                advanceBases(two_base, score, true);
                advanceBases(thr_base, score, true);
                thr_base = players[now_batter].name; 
                break;
        }
        base++; // ベースを増やす
    }
}

// 打席結果をランダムに生成する関数
// Microsoft Copilot AIによって生成した部分です
int generateAtBatResult(const PlayerStats& player) {
    // 乱数生成器の初期化
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // 打率に基づくヒットの確率
    std::bernoulli_distribution hitDist(player.avg);
    
    // 長打率に基づく長打の確率
    double extraBaseHitProb = player.slg - player.avg;
    std::bernoulli_distribution extraBaseHitDist(extraBaseHitProb);
    
    // 打席結果の生成
    if (hitDist(gen)) {
        // ヒットが出た場合
        if (extraBaseHitDist(gen)) {
            // 長打の確率に基づいて二塁打、三塁打、本塁打を決定
            std::uniform_int_distribution<> extraBaseHitType(2, 4);
            return extraBaseHitType(gen); // 2:二塁打, 3:三塁打, 4:本塁打
        } else {
            return 1; // 単打
        }
    } else {
        // ヒットが出なかった場合
        // ランダムな打席結果を生成
        int result = rand() % 4 ; // 0:フライ, 1:ゴロ, 2:三振, 3:四球
        switch(result) {
            case 0: return 5; // フライ
            case 1: return 6; // ゴロ
            case 2: return 7; // 三振
            case 3: return 0; // 四球
        }
    }
    return 0;
}

// 選手一覧から選手データを取得し、すべてを表示する関数
void getPlayerData(std::vector<PlayerStats> &players) {
    // 名前, 打率(AVG), 長打率(SLG), 打数(AB), 安打数(H), 盗塁数(SB), 出塁率(OBP)

    std::ifstream file("roster.csv"); // CSVファイルを開く
    // 構造体の配列
    std::string line, word;

    // CSVファイルのヘッダーをスキップ
    std::getline(file, line);

    // CSVファイルの各行を読み込む
    while (std::getline(file, line)) {
        std::stringstream s(line);
        PlayerStats player;

        // 名前
        std::getline(s, player.name, ',');

        // 背番号
        std::getline(s, word, ',');
        player.num = std::stoi(word);

        // 打率(AVG)
        std::getline(s, word, ',');
        player.avg = std::stod(word);

        // 長打率(SLG)
        std::getline(s, word, ',');
        player.slg = std::stod(word);

        // 打数(AB)
        std::getline(s, word, ',');
        player.ab = std::stoi(word);

        // 安打数(H)
        std::getline(s, word, ',');
        player.h = std::stoi(word);

        // 盗塁数(SB)
        std::getline(s, word, ',');
        player.sb = std::stoi(word);

        // 出塁率(OBP)
        std::getline(s, word);
        player.obp = std::stod(word);

        // 構造体の配列に追加
        players.push_back(player);
    }
}

void showAllPlayers(std::vector<PlayerStats> &players) {
    for (const auto &player : players) {
        std::cout << "名前: " << player.name << std::endl;
        std::cout << "背番号: " << player.num << std::endl;
        std::cout << "打率: " << player.avg << std::endl;
        std::cout << "長打率: " << player.slg << std::endl;
        std::cout << "打数: " << player.ab << std::endl;
        std::cout << "安打数: " << player.h << std::endl;
        std::cout << "盗塁数: " << player.sb << std::endl;
        std::cout << "出塁率: " << player.obp << std::endl;
        std::cout << std::endl;
    }
}

void showOrder(std::vector<PlayerStats> &players) {
    for (int i = 0; i < 9; i++) {
        std::cout << "打順: " << i + 1 << " " << players[i].name << std::endl;
    }
}

// 選手を手動で選択する関数
std::vector<PlayerStats> selectPlayersManually(std::vector<PlayerStats>& players) {
    std::vector<PlayerStats> selectedPlayers;
    int playerNum = 0;
    int count = 0;

    while (count < 9) {
        std::cout << "選手の背番号を入力してください: ";
        cin >> playerNum;

        // 選手を検索してリストに追加
        for (const auto& player : players) {
            if (player.num == playerNum) {
                selectedPlayers.push_back(player);
                count++;
                break;
            }
        }

    }

    return selectedPlayers;
}

int main() {
    int out_count = 0;

    // 選手データを取得
    std::vector<PlayerStats> players;
    getPlayerData(players);
    showAllPlayers(players);

    // ユーザーが選手を手動で選択
    std::vector<PlayerStats> selectedPlayers = selectPlayersManually(players);
    // 選択された選手を表示
    showAllPlayers(selectedPlayers);

    string sign = "n";
    string batresult = "";
    int now_batter = -1;
    while(selectedPlayers.size() == 9 && out_count < 3) {
        if(now_batter == 8) now_batter = 0;
        else now_batter++;

        // 打席結果を生成
        int result = generateAtBatResult(selectedPlayers[now_batter]);

        // 打者が打席に立つ
        cout << "打者：" << selectedPlayers[now_batter].name << endl;

        cout << "サインを出す場合、サインを入力してください" << endl;
        cout << "出さない場合はnを押してください" << endl;
        // サインを入力
        cin >> sign;

        // サインに応じて打席結果を表示
        if(sign == "n") {
            if(result == 5 || result == 6 || result == 7) {
                out_count++;
            }
            else {
                hitResult(result, selectedPlayers, now_batter);
            }
            // 打席内容を表示
            batresult = getAtBatResultString(result);
            cout << "打者：" << selectedPlayers[now_batter].name << "の結果：" << batresult << endl;
            cout << out_count << "アウト" << " ";
            cout << "現在の得点：" << score << endl;
            cout << endl;
        } else {
            cout << "打者に" << sign << "を要求します。" << endl;
        }

        // 各塁の状況を表示
        cout << "一塁：" << one_base << endl;
        cout << "二塁：" << two_base << endl;
        cout << "三塁：" << thr_base << endl;
        cout << endl;
    }
    cout << "チェンジ！" << endl;
}