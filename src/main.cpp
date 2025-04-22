#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QString>
#include <QStack>
#include <QJSEngine>

class Calculator : public QWidget {
    public:
        Calculator(QWidget *parent = nullptr);
    private:
        QLineEdit *display;
        QString currentInput;
        QStack<QString> history;
        void buttonClicked();
        void evaluatedExpression();
        void clear();
};

/*
    QWidget 相続, 現在インプット初期化
 */
Calculator::Calculator(QWidget *parent) : QWidget(parent), currentInput("") {
    QVBoxLayout *mainLayout = new QVBoxLayout;

    display = new QLineEdit;
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    mainLayout->addWidget(display);

    /*
        ボタン配置
    */
    QGridLayout *gridLayout = new  QGridLayout;

    QString buttonText[5][4] = {
        {"7", "8", "9", "/"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {".", "0", "=", "+"},
        {"C", "(", ")", "Del"}
    };

    QPushButton *buttons[5][4];

    /*
        ボタンラベリング、slot関数登録
    */
    for (int row = 0; row < 5; row++) {
        for(int col = 0; col < 4; col++) {
            buttons[row][col] = new QPushButton(buttonText[row][col]);
            gridLayout->addWidget(buttons[row][col], row, col);
            connect(buttons[row][col], &QPushButton::clicked, this, &Calculator::buttonClicked);
        }
    }

    mainLayout->addLayout(gridLayout);
    setLayout(mainLayout);
    setWindowTitle("Calculator");
    resize(300,350);
}

/*
  数字入力  
*/
void Calculator::buttonClicked() {
    QString expression = display->text();

    if (expression == "Error") {
        display->clear();
    }
    
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button)
        return;
    
    QString buttonText = button->text();

    /*
        入力クリア
    */
    if (buttonText == "C") {
        currentInput.clear();
        display->clear();
        return;
    }

    /*
        入力削除
    */
    if (buttonText == "Del") {
        QString text = display->text();
    
        if (!text.isEmpty()) {
            text.chop(1);
            display->setText(text);
            currentInput = text; 
        }
        return;
    }

    /*
        式計算
    */
    if (buttonText == "=") {
        evaluatedExpression();
        return;
    }

    /*
        displayに表示、インプットクリア
    */
    currentInput += buttonText;
    display->setText(currentInput);
}

void Calculator::evaluatedExpression() {
    QString expression = display->text();

    //Errorの場合クリア
    if (expression == "Error") {
        return;
    }

    bool isNumeric = false;
    expression.toDouble(&isNumeric);

    //計算
    if (isNumeric) {
        display->setText(expression);
    } else {
        QJSEngine engine;
        QJSValue result = engine.evaluate(expression);

        if (result.isError() || result.isUndefined()) {
            display->setText("Error");
            currentInput.clear();
        } else {
            display->setText(result.toString());
            currentInput = display->text();
        }
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Calculator calculator;
    calculator.show();

    return app.exec();
}