#include "mainwindow.h"
#include <algorithm>
#include <QVariant>
#include <QPixmap>
#include <QDebug>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QTimer>
#include <array>
#include <vector>
#include <random>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      rowCount(4),
      colCount(5),
      totalIcons(10),
      moves(0),
      matches(0),
      firstSelected(-1),
      secondSelected(-1),
      currentPlayer(1),
      scorePlayer1(0),
      scorePlayer2(0),
      playerMode(0),
      moveLimit(20)
{
    initializeIcons();
    createWidgets();
    setupLayout();
    connectSlots();
    setupBoard();
    setupGameModeButtons();

    setStyleSheet("background-color: lightyellow;");

}

void MainWindow::setupGameModeButtons()
{
   groupBox = new QGroupBox(tr("Game Modes"), this);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox_temp = vbox;
    singlePlayerButton = new QPushButton("Single Player", this);
    singlePlayerButton->setStyleSheet("background-color: orange;");
    connect(singlePlayerButton, &QPushButton::clicked, this, &MainWindow::setSinglePlayerMode);
    twoPlayersButton = new QPushButton("Two Players", this);
    twoPlayersButton->setStyleSheet("background-color: orange;");
    connect(twoPlayersButton, &QPushButton::clicked, this, &MainWindow::setTwoPlayersMode);

    vbox->addWidget(singlePlayerButton);
    vbox->addWidget(twoPlayersButton);
    vbox->addWidget(newGameButton);
    groupBox->setLayout(vbox);

    gridLayout->addWidget(groupBox, rowCount + 1, 0, 1, colCount);
}

void MainWindow::setSinglePlayerMode()
{
    removeWidgetsFromLayout();
    movesLeftLabel = new QLabel("Moves Left: ", this);
    vbox_temp->addWidget(movesLeftLabel);
    vbox_temp->addWidget(singlePlayerButton);
    vbox_temp->addWidget(twoPlayersButton);
    vbox_temp->addWidget(newGameButton);
    groupBox->setLayout(vbox_temp);
    movesLeftLabel->setText("Moves Left: " + QString::number(moveLimit));
    playerMode = 1;
    QMessageBox::information(this, "Game Mode", "Single Player Mode selected.");

}

void MainWindow::setTwoPlayersMode()
{
    removeWidgetsFromLayout();
    Player1Score = new QLabel("Player 1 Score: ", this);
    Player2Score = new QLabel("Player 2 Score: ", this);
    Turn = new QLabel("Turn: ", this);
    vbox_temp->addWidget(movesLeftLabel);
    vbox_temp->addWidget(Turn);
    vbox_temp->addWidget(Player1Score);
    vbox_temp->addWidget(Player2Score);
    vbox_temp->addWidget(singlePlayerButton);
    vbox_temp->addWidget(twoPlayersButton);
    vbox_temp->addWidget(newGameButton);
    groupBox->setLayout(vbox_temp);



    playerMode = 2;
    QMessageBox::information(this, "Game Mode", "Two Players Mode selected.");
    movesLeftLabel->setText("Moves Left: Unlimited");
    if (playerMode==2){
    if (currentPlayer == 1){

    Turn->setText("Turn: Player " + QString::number(currentPlayer));
    }
    else{
    Turn->setText("Turn: Player " + QString::number(currentPlayer));
    }

    }

}
// Assuming you have access to pointers of the widgets and layout
// Let's call this function when you want to remove the widgets

void MainWindow::removeWidgetsFromLayout() {
    // Remove widgets from layout
    vbox_temp->removeWidget(movesLeftLabel);
    vbox_temp->removeWidget(Turn);
    vbox_temp->removeWidget(Player1Score);
    vbox_temp->removeWidget(Player2Score);

}


void MainWindow::onCardClicked() {

    QPushButton *cardButton = qobject_cast<QPushButton *>(sender());
    int index = cardButton->property("index").toInt();

    if (playerMode == 0) {
        QMessageBox::information(this, "Game Mode", "Please select a game mode before playing!");
        return;
      }

    if (index < 0 || index >= totalIcons * 2 || revealed[index] || matched[index])
        return;

    cardButton->setIcon(icons[index]);
    revealed[index] = true;
    if (playerMode==2){
    if (currentPlayer == 1){

    Turn->setText("Turn: Player " + QString::number(currentPlayer));
    }
    else{


    Turn->setText("Turn: Player " + QString::number(currentPlayer));
    }
    }
    if (firstSelected == -1) {
        firstSelected = index;
    } else {
        secondSelected = index;
        moves++;

        if (icons[firstSelected].toImage() == icons[secondSelected].toImage()) {
            matched[firstSelected] = matched[secondSelected] = true;
            matches++;
            if (playerMode == 2) {

                if (currentPlayer == 1){
                    scorePlayer1++;
                Turn->setText("Turn: Player " + QString::number(currentPlayer+1));
                }
                else{
                    scorePlayer2++;

                Turn->setText("Turn: Player " + QString::number(currentPlayer-1));
                }
                Player1Score->setText("Player 1 Score: " + QString::number(scorePlayer1));
                Player2Score->setText("Player 2 Score: " + QString::number(scorePlayer2));
            }
            if (playerMode == 1) {
                moveLimit--; // Decrease move limit if both cards are selected and they are different
                movesLeftLabel->setText("Moves Left: " + QString::number(moveLimit));
        }

            processMatchedCards(firstSelected, secondSelected);

            if (matches == totalIcons) {
                if (playerMode == 1)
                    QMessageBox::information(this, "Game Over", "Congratulations! You've matched all icons in " + QString::number(moves) + " moves.");

                else
                    QMessageBox::information(this, "Game Over", "Congratulations! Player " + QString::number(scorePlayer1 > scorePlayer2 ? 1 : 2) + " wins with a score of " + QString::number(scorePlayer1 > scorePlayer2 ? scorePlayer1 : scorePlayer2));
                resetGame();
            }
            firstSelected = secondSelected = -1;
        } else {
            QTimer::singleShot(1000, this, &MainWindow::hideSelectedCards);
            if (playerMode == 2) {

                currentPlayer = (currentPlayer == 1) ? 2 : 1;

                Player1Score->setText("Player 1 Score: " + QString::number(scorePlayer1));
                Player2Score->setText("Player 2 Score: " + QString::number(scorePlayer2));

            }
            if (playerMode == 1) {
                moveLimit--; // Decrease move limit if both cards are selected and they are different
                movesLeftLabel->setText("Moves Left: " + QString::number(moveLimit));
                if (moveLimit == 0 && matches != totalIcons){
                    QMessageBox::information(this, "Game Over", "You don't have any moves left.");
                    resetGame();
                }
        }
    }
}
}

void MainWindow::processMatchedCards(int firstIndex, int secondIndex) {
     buttons[firstIndex]->setStyleSheet("background-color: green");
     buttons[secondIndex]->setStyleSheet("background-color: green");
}

void MainWindow::initializeIcons()
{
    std::vector<QPixmap> tempIcons;
    for (int i = 1; i <= totalIcons; ++i)
    {
        QString imagePath = ":/img/images/icon" + QString::number(i) + ".png";
        QPixmap pixmap(imagePath);
        if (pixmap.isNull()) {
            qDebug() << "Failed to load image:" << imagePath;
            continue;
        }
        tempIcons.push_back(pixmap);
    }

    icons.resize(totalIcons * 2);
    std::copy(tempIcons.begin(), tempIcons.end(), icons.begin());
    std::copy(tempIcons.begin(), tempIcons.end(), icons.begin() + totalIcons);

    // Shuffle the icons using std::shuffle and a random number generator
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(icons.begin(), icons.end(), g);
}

void MainWindow::createWidgets()
{
    gridLayout = new QGridLayout(this);
    newGameButton = new QPushButton("New Game", this);
    newGameButton->setStyleSheet("background-color: orange;");
    connect(newGameButton, &QPushButton::clicked, this, &MainWindow::resetGame);

    for (int row = 0; row < rowCount; ++row)
    {
        for (int col = 0; col < colCount; ++col)
        {
            QPushButton *cardButton = new QPushButton(this);
            cardButton->setIconSize(QSize(100, 100));
            cardButton->setProperty("index", QVariant(row * colCount + col));
            cardButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed); // Set size policy
            cardButton->setFixedSize(100, 100); // Set fixed size

            gridLayout->addWidget(cardButton, row, col);
            buttons.push_back(cardButton);
        }
    }
}


void MainWindow::setupLayout()
{
    setLayout(gridLayout);
}

void MainWindow::connectSlots()
{
    for (QPushButton* button : buttons)
    {
        connect(button, &QPushButton::clicked, this, &MainWindow::onCardClicked);
    }
}

void MainWindow::setupBoard()
{
    revealed.assign(totalIcons * 2, false);
    matched.assign(totalIcons * 2, false);
}

void MainWindow::hideSelectedCards()
{
    if (firstSelected < 0 || firstSelected >= int(buttons.size()) ||
        secondSelected < 0 || secondSelected >= int(buttons.size())) {
        return;
    }

    buttons[firstSelected]->setIcon(QIcon());
    buttons[secondSelected]->setIcon(QIcon());
    revealed[firstSelected] = false;
    revealed[secondSelected] = false;

    firstSelected = secondSelected = -1;
}

void MainWindow::resetGame() {
    initializeIcons();
    for (int i = 0; i < int(buttons.size()); ++i) {
            buttons[i]->setIcon(QIcon());
            revealed[i] = false;
            matched[i] = false;
            buttons[i]->setStyleSheet(""); // Reset button styles (removes green background)

    }

    moves = 0;
    matches = 0;
    firstSelected = -1;
    secondSelected = -1;
    currentPlayer = 1; // Reset to player 1 in case of two-player mode
    scorePlayer1 = 0;
    scorePlayer2 = 0;


if(playerMode == 1){
    moveLimit = 20;
    movesLeftLabel->setText("Moves Left: 20");
} else {
    movesLeftLabel->setText("Moves Left: Unlimited");
        // Update player turn label based on new currentPlayer
        Turn->setText("Turn: Player 1");
        // Update player score labels
        Player1Score->setText("Player 1 Score: " + QString::number(scorePlayer1));
        Player2Score->setText("Player 2 Score: " + QString::number(scorePlayer2));
}
}
