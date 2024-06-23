#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib> // std::rand, std::srand
#include <chrono>
#include <random> // Add this line to include the <random> header
using namespace std;

/*
0: アウト
1: 単打
2: 二塁打
3: 三塁打
4: ホームラン
*/

// 乱数生成器の初期化
std::random_device rd;
std::mt19937 gen(rd());

class PlayerStats {
    private:
        std::string name;
        int num;
        double avg;
        double slg;
        int ab;
        int h;
        int sb;
        double obp;
    public:
        // デフォルトコンストラクタ
        PlayerStats() : name(""), num(0), avg(0.0), slg(0.0), ab(0), h(0), sb(0), obp(0.0) {}

    // データを設定するセッター関数
    void setName(std::string newName) { name = newName; }
    void setNum(int newNum) { num = newNum; }
    void setAvg(double newAvg) { avg = newAvg; }
    void setSlg(double newSlg) { slg = newSlg; }
    void setAb(int newAb) { ab = newAb; }
    void setH(int newH) { h = newH; }
    void setSb(int newSb) { sb = newSb; }
    void setObp(double newObp) { obp = newObp; }

    // データを取得するゲッター関数
    std::string getName() const { return name; }
    int getNum() const { return num; }
    double getAvg() const { return avg; }
    double getSlg() const { return slg; }
    int getAb() const { return ab; }
    int getH() const { return h; }
    int getSb() const { return sb; }
    double getObp() const { return obp; }

    // 打率を更新する関数
    void updateAvg(int newHits, int newAtBats) {
        addNewHits(newHits);
        addNewAtBats(newAtBats);
    }

    void addNewHits(int newHits) {
        h += newHits;
        avg = static_cast<double>(h) / ab;
    }

    void addNewAtBats(int newAtBats) {
        ab += newAtBats;
        avg = static_cast<double>(h) / ab;
    }
    // OPS (出塁率 + 長打率) を計算する関数
    double calculateOps() const {
        return obp + slg;
    }

    void showPlayers() {
        std::cout << "背番号" << num << " ";
        std::cout << name << " ";
        std::cout << avg << " ";
        std::cout << h << "/" << ab << " ";
        std::cout << sb << "盗塁" << " ";
        std::cout << "OPS: " << obp+slg << std::endl;
        std::cout << std::endl;
    }
};

class PitcherStats {
    private:
        std::string name;
        // 背番号
        int num;
        // 防御率
        double era;
        // 投球回
        int ip;
        // 被安打
        int h;
        // 与四球
        int bb;
        // 三振
        int so;
        // 最高球速
        int max_speed;
        // 疲労度
        int fatigue;
    
    public:
        // デフォルトコンストラクタ
        PitcherStats() : name(""), num(0), era(0.0), ip(0), h(0), bb(0), so(0) {}

        // データを設定するセッター関数
        void setName(std::string newName) { name = newName; }
        void setNum(int newNum) { num = newNum; }
        void setEra(double newEra) { era = newEra; }
        void setIp(int newIp) { ip = newIp; }
        void setH(int newH) { h = newH; }
        void setBb(int newBb) { bb = newBb; }
        void setSo(int newSo) { so = newSo; }

        // データを取得するゲッター関数
        std::string getName() const { return name; }
        int getNum() const { return num; }
        double getEra() const { return era; }
        int getIp() const { return ip; }
        int getH() const { return h; }
        int getBb() const { return bb; }
        int getSo() const { return so; }

        // 防御率を更新する関数
        void updateEra(int newEarnedRuns, int newInningsPitched) {
            addNewEarnedRuns(newEarnedRuns);
            addNewInningsPitched(newInningsPitched);
        }

        void addNewEarnedRuns(int newEarnedRuns) {
            era = static_cast<double>(newEarnedRuns) / ip * 9;
        }

        void addNewInningsPitched(int newInningsPitched) {
            ip += newInningsPitched;
            era = static_cast<double>(h) / ip * 9;
        }

        // K/BB比を計算する関数
        double getkbb() {
            double kbb = so / bb;
            return kbb;
        }

        void showPlayers() {
            std::cout << "背番号" << num << " ";
            std::cout << name << " ";
            std::cout << "防御率：" << era << " ";
            std::cout << ip << "回 ";
            std::cout << h << "被安打 ";
            std::cout << bb << " ";
            std::cout << so << " ";
            std::cout << "K/BB: " << so / bb << std::endl;
            std::cout << std::endl;
        }

        void updateFatigue() {
            fatigue = 0;
        }
};

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
int hitResult(int hitType, std::vector<PlayerStats> &players, int &now_batter, int &score, int &ball, int &strike, int &out_count) {
    // return 0のとき：
    if(ball<3 && hitType == 8) {
        // ボールの場合、ボールを進める
        ball++;
        return 0;
    }
    else if(strike<2 && hitType == 7 || hitType == 9) {
        // ストライクの場合、アウトカウントを増やす
        strike++;
        return 0;
    }
    else if(hitType >= 5) {
        players[now_batter].addNewAtBats(1);
        out_count++;
        return 1; // アウトの場合、何もしない
    }
    else {
        // 打数・安打数・打率を更新
        players[now_batter].updateAvg(0, 1);
        // ホームランの場合、全てのランナーがスコアする
        // ベースを進める
        if(hitType >= 0) pushRunner(two_base, thr_base, score);
        if(hitType >= 2) pushRunner(one_base, two_base, score);

        // ヒットに応じてランナーを配置
        switch(hitType) {
            // 四球の場合
            case 8: 
                // 2から3にランナーを進める
                pushRunner(two_base, thr_base, score);
                // 1から2にランナーを進める
                pushRunner(one_base, two_base, score);
                advanceBases(thr_base, score, true);
                one_base = players[now_batter].getName(); 
                break;
            // 単打の場合
            case 1: 
                // 2から3にランナーを進める
                pushRunner(two_base, thr_base, score);
                // 1から2にランナーを進める
                pushRunner(one_base, two_base, score);
                advanceBases(thr_base, score, true);
                one_base = players[now_batter].getName(); 
                break;
            // 二塁打の場合
            case 2: 
                // 1から3にランナーを進める
                pushRunner(one_base, thr_base, score);
                // 2,3塁ランナーをホームイン
                advanceBases(two_base, score, true);
                advanceBases(thr_base, score, true);
                two_base = players[now_batter].getName(); 
                break;
            // 三塁打の場合
            case 3:
                // 1,2,3からホームにランナーを進める
                advanceBases(one_base, score, true);
                advanceBases(two_base, score, true);
                advanceBases(thr_base, score, true);
                thr_base = players[now_batter].getName(); 
                break;
            // ホームランの場合
            case 4:
                advanceBases(one_base, score, true);
                advanceBases(two_base, score, true);
                advanceBases(thr_base, score, true);
                score++; // バッターもスコアする
                base = 0; // ベースをクリア
                break;
            // ゴロの場合、ランナーを進める
            case 6:
                // 2から3にランナーを進める
                pushRunner(two_base, thr_base, score);
                // 3からホームにランナーを進める
                advanceBases(thr_base, score, true);
                // 1から2にランナーを進める
                pushRunner(one_base, two_base, score);
                break;
                
        }
    }
    return 1;
}

// 打席結果を文字列で返す関数
string getAtBatResultString(int result) {
    switch (result) {
        case 0: return "ボール";
        case 1: return "単打";
        case 2: return "二塁打";
        case 3: return "三塁打";
        case 4: return "ホームラン";
        case 5: return "フライ";
        case 6: return "ゴロ";
        case 7: return "空振り";
        case 8: return "ボール";
        case 9: return "ファウル";
        default: return "不明な結果";
    }
}

// 長打の種類を決定する関数
int determineExtraBaseHit(const PlayerStats& player, std::mt19937& gen) {
    std::uniform_int_distribution<> extraBaseHitType(2, 4);
    return extraBaseHitType(gen); // 2:二塁打, 3:三塁打, 4:本塁打
}

// アウトの種類を決定する関数
int determineOutType(std::mt19937& gen) {
    std::uniform_int_distribution<> outType(5, 6);
    return outType(gen); // 5:フライ, 6:ゴロ
}


// 打席結果をランダムに生成する関数
int generateAtBatResult(PlayerStats& player, int ball, int strike, std::string sign) {
    // 乱数シードの設定
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed); // 乱数生成器

    std::discrete_distribution<> atBatResultDist; // Declare the variable "atBatResultDist"
    
    // 打席結果の確率分布を設定
    atBatResultDist = std::discrete_distribution<> ({
        player.getAvg() - player.getObp(), // 単打
        player.getSlg() - player.getAvg(), // 長打（二塁打、三塁打、本塁打）
        1 - player.getObp(), // アウト（ゴロ、フライ、三振）
        player.getObp() - player.getAvg(), // ボール
        player.getObp() - player.getAvg(), // 空振り
        player.getObp(), // ファウル
    });
    
    // 「振りに行け」サインが出された場合の確率分布の調整
    double hitIncreaseFactor = (sign=="h") ? 1.2 : 1.0; // ヒット確率を20%増加
    double strikeOutIncreaseFactor = (sign=="h") ? 1.1 : 1.0; // 三振確率を10%増加

    // 打席結果をランダムに生成
    int atBatResult = atBatResultDist(gen);

    // カウントに応じた追加のロジック
    if (strike == 2) {
        // ストライクが2の場合、三振の確率を増加
        atBatResult = (atBatResult == 2) ? 6 : atBatResult;
    } else if (ball == 3) {
        // ボールが3の場合、四球の確率を増加
        atBatResult = (atBatResult == 3) ? 7 : atBatResult;
    }

    // 打席結果に応じた処理
    switch (atBatResult) {
        case 0: // 単打
            player.updateAvg(1, 1);
            return 1;
        case 1: // 長打
            player.updateAvg(1, 1); // 打率を更新
            // 長打の種類を決定する追加のロジック
            return determineExtraBaseHit(player, gen);
        case 2: // アウト
            player.addNewAtBats(1);
            return determineOutType(gen);
        case 3: // ボール
            return 8;
        case 4: // ファウル
            if (strike < 2) {
                strike++;
            }
            return 9;
        case 5: // 空振り
            return 7;
        default:
            return 0; // 予期せぬ結果
    }
}

// 選手一覧から選手データを取得し、すべてを表示する関数
void getPlayerData(std::vector<PlayerStats> &players, std::string filename) {
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

        std::string tmpword;
        // 名前
        std::getline(s, tmpword, ',');
        player.setName(tmpword);

        // 背番号
        std::getline(s, word, ',');
        player.setNum(std::stoi(word));

        // 打率(AVG)
        std::getline(s, word, ',');
        player.setAvg(std::stod(word));

        // 長打率(SLG)
        std::getline(s, word, ',');
        player.setSlg(std::stod(word));

        // 打数(AB)
        std::getline(s, word, ',');
        player.setAb(std::stod(word));

        // 安打数(H)
        std::getline(s, word, ',');
        player.setH(std::stod(word));

        // 盗塁数(SB)
        std::getline(s, word, ',');
        player.setSb(std::stod(word));

        // 出塁率(OBP)
        std::getline(s, word);
        player.setObp(std::stod(word));

        // 構造体の配列に追加
        players.push_back(player);
    }
}

void showAllPlayers(std::vector<PlayerStats> &players) {
    for (PlayerStats &player : players) {
        player.showPlayers();
    }
}

void showOrder(std::vector<PlayerStats> &players) {
    for (int i = 0; i < 9; i++) {
        std::cout << "打順: " << i + 1 << " " << players[i].getName() << std::endl;
    }
}

// 選手を手動で選択する関数
std::vector<PlayerStats> selectPlayersManually(std::vector<PlayerStats>& players) {
    std::vector<PlayerStats> selectedPlayers;
    int playerNum = 0;
    int count = 0;

    while (count < 9) {
        std::cout << "選手の背番号を入力してください: ";
        std::cin >> playerNum;

        // 選手を検索してリストに追加
        for (const auto& player : players) {
            if (player.getNum() == playerNum) {
                selectedPlayers.push_back(player);
                count++;
                break;
            }
        }

    }

    return selectedPlayers;
}

void updateRoster(std::vector<PlayerStats>& players, std::vector<PlayerStats>& selectedPlayers) {
    std::ofstream file("roster.csv"); // CSVファイルを開く
    file << "名前, 背番号, 打率(AVG), 長打率(SLG), 打数(AB), 安打数(H), 盗塁数(SB), 出塁率(OBP)" << std::endl;
    for (const auto& player : players) {
        bool found = false;
        for (const auto& selectedPlayer : selectedPlayers) {
            if (player.getNum() == selectedPlayer.getNum()) {
                file << selectedPlayer.getName() << ", " << selectedPlayer.getNum() << ", "
                << selectedPlayer.getAvg() << ", " << selectedPlayer.getSlg() << ", " << selectedPlayer.getAb() << ", "
                << selectedPlayer.getH() << ", " << selectedPlayer.getSb() << ", " << selectedPlayer.getObp() << std::endl;
                found = true;
                break;
            }
        }
        if (!found) {
            file << player.getName() << ", " << player.getNum() << ", " 
            << player.getAvg() << ", " << player.getSlg() << ", " << player.getAb() << ", " 
            << player.getH() << ", " << player.getSb() << ", " << player.getObp() << std::endl;
        }
    }
}

int main() {
    int out_count = 0;
    int ball = 0, strike = 0;

    // 選手データを取得
    std::vector<PlayerStats> players;
    getPlayerData(players, "roster.csv");
    showAllPlayers(players);

    std::cout << "打順を選択してください。" << endl;
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

        // 打者が打席に立つ
        std::cout << "打者：" << selectedPlayers[now_batter].getName() << std::endl;

        // カウントが溜まるまで打席を続行
        while(true) {
            // ストライクとボールを表示
            std::cout << "B:";
            for(int i = 0; i < ball; i++) std::cout << "●";
            std::cout << "\nS:";
            for(int i = 0; i < strike; i++) std::cout << "●";
            std::cout << "\nO:";
            for(int i = 0; i < out_count; i++) std::cout << "●";
            std::cout << std::endl;

            std::cout << "サインを出す場合、サインを入力してください" << std::endl;
            std::cout << "出さない場合はnを押してください" << std::endl;
            // サインを入力
            std::cin >> sign;
            cout << endl;

            if(strike == 2 && ball == 3) {
                std::cout << "フルカウント！" << std::endl;
            }
            // 打席結果を文字に変換
            int result = generateAtBatResult(selectedPlayers[now_batter], ball, strike, sign);
            std::string resultString = getAtBatResultString(result);
            std::cout << resultString << std::endl;
            if(hitResult(result, selectedPlayers, now_batter, score, ball, strike, out_count)==1) {
                break;
            }
            
        }

        strike = 0;
        ball = 0;

        // 各塁の状況を表示
        std::cout << "一塁：" << one_base << std::endl;
        std::cout << "二塁：" << two_base << std::endl;
        std::cout << "三塁：" << thr_base << std::endl;
        std::cout << std::endl;
    }
    std::cout << "チェンジ！" << std::endl;
    updateRoster(players, selectedPlayers);
}