#ifndef   PLAYERSTATS_HPP
#define   PLAYERSTATS_HPP
#include <string>
#include <iostream>

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

#endif // PLAYERSTATS_HPP