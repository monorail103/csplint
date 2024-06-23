#ifndef   PITCHERSTATS_HPP
#define   PITCHERSTATS_HPP
#include <string>
#include <iostream>

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

#endif // PITCHERSTATS_HPP