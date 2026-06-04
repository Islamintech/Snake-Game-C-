// ============================================================
//  scoreboard.h
//  ScoreBoard class: tracks game statistics, per-stage missions
//  and renders the score / mission side panels.
// ============================================================
#ifndef SCOREBOARD_H
#define SCOREBOARD_H

// Mission targets for a single stage.
struct Mission {
    int maxLength;   // target snake length (B)
    int growth;      // growth items to eat (+)
    int poison;      // poison items to eat (-)
    int gate;        // gate passes (G)
};

class ScoreBoard {
public:
    ScoreBoard();

    void reset(int stage);

    // Stat updates.
    void onGrowth();
    void onPoison();
    void onGate();
    void setLength(int len);
    void tickSeconds(int elapsedSeconds);

    bool missionComplete() const;

    // Render score + mission panels starting at screen column `originX`.
    void draw(int originX, int stage) const;

    int  maxLength() const { return maxLength_; }

private:
    int curLength_;
    int maxLength_;
    int growth_;
    int poison_;
    int gate_;
    int seconds_;
    Mission mission_;

    static Mission missionFor(int stage);
};

#endif // SCOREBOARD_H
