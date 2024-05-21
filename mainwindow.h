#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QIcon>
#include <QTimer>
#include <vector>
#include <QLabel>
#include <QVBoxLayout>
#include <QGroupBox>

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onCardClicked();
    void setSinglePlayerMode();
    void setTwoPlayersMode();

private:
    int rowCount;
    int colCount;
    int totalIcons;
    int moves;
    int matches;
    int count = 0;
    int firstSelected;
    int secondSelected;
    int currentPlayer;
    int scorePlayer1;
    int scorePlayer2;
    int playerMode; // 1 for single player, 2 for two players
    int moveLimit;
    std::vector<QPixmap> icons;
    std::vector<bool> revealed;
    std::vector<bool> matched;
    QGridLayout *gridLayout;
    std::vector<QPushButton*> buttons;
    QLabel *movesLabel;
    QLabel *titleLabel;
    QPushButton *singlePlayerButton;
    QPushButton *twoPlayersButton;
    QLabel *movesLeftLabel;
    QLabel *Player1Score;
    QLabel *Player2Score;
    QLabel *Turn;
    QPushButton *newGameButton;
    QVBoxLayout *v2;
    QGroupBox *groupBox;
    QVBoxLayout *vbox_temp;

    void initializeIcons();
    void createWidgets();
    void removeWidgetsFromLayout();
    void setupLayout();
    void connectSlots();
    void setupBoard();
    void hideSelectedCards();
    void processMatchedCards(int, int);
    void updateMovesLabel();
    void resetGame();
    void addTitle();
    void setupGameModeButtons();
};

#endif // MAINWINDOW_H
