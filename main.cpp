#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QWidget>
#include <QString>
#include <QVector>
#include <QStack>
#include <QDebug>
#include <cctype>
#include <stdexcept>

class MathParser {
public:
    static double evaluate(const QString& expression) {
        QVector<QString> tokens = tokenize(expression);
        QVector<QString> postfix = shuntingYard(tokens);
        return evaluatePostfix(postfix);
    }

private:
    static int getPrecedence(const QString& op) {
        if (op == "+" || op == "-") return 1;
        if (op == "*" || op == "/") return 2;
        return 0;
    }

    static QVector<QString> tokenize(const QString& expr) {
        QVector<QString> tokens;
        QString numberBuffer;

        for (int i = 0; i < expr.length(); ++i) {
            QChar ch = expr[i];

            if (ch.isDigit() || ch == '.') {
                numberBuffer.append(ch);
            } else {
                if (!numberBuffer.isEmpty()) {
                    tokens.append(numberBuffer);
                    numberBuffer.clear();
                }
                if (!ch.isSpace()) {
                    tokens.append(QString(ch));
                }
            }
        }
        if (!numberBuffer.isEmpty()) {
            tokens.append(numberBuffer);
        }
        return tokens;
    }

    static QVector<QString> shuntingYard(const QVector<QString>& tokens) {
        QVector<QString> output;
        QStack<QString> operators;

        for (const QString& token : tokens) {
            bool isNumber;
            token.toDouble(&isNumber);

            if (isNumber) {
                output.append(token);
            } else if (token == "(") {
                operators.push(token);
            } else if (token == ")") {
                while (!operators.isEmpty() && operators.top() != "(") {
                    output.append(operators.pop());
                }
                if (!operators.isEmpty()) operators.pop(); // Remove '('
            } else { // Operator
                while (!operators.isEmpty() && getPrecedence(operators.top()) >= getPrecedence(token)) {
                    output.append(operators.pop());
                }
                operators.push(token);
            }
        }

        while (!operators.isEmpty()) {
            output.append(operators.pop());
        }

        return output;
    }

    static double evaluatePostfix(const QVector<QString>& postfix) {
        QStack<double> stack;

        for (const QString& token : postfix) {
            bool isNumber;
            double value = token.toDouble(&isNumber);

            if (isNumber) {
                stack.push(value);
            } else {
                if (stack.size() < 2) throw std::runtime_error("Malformed Expression");
                double val2 = stack.pop();
                double val1 = stack.pop();

                if (token == "+") stack.push(val1 + val2);
                else if (token == "-") stack.push(val1 - val2);
                else if (token == "*") stack.push(val1 * val2);
                else if (token == "/") {
                    if (val2 == 0) throw std::runtime_error("Division by Zero");
                    stack.push(val1 / val2);
                }
            }
        }

        if (stack.size() != 1) throw std::runtime_error("Malformed Expression");
        return stack.pop();
    }
};

class Calculator : public QWidget {
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Engineered Custom Calculator");
        setFixedSize(320, 450);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        inputField = new QLineEdit();
        inputField->setAlignment(Qt::AlignRight);
        inputField->setReadOnly(true);
        inputField->setStyleSheet("font-size: 26px; height: 60px; padding: 5px; font-weight: bold;");
        mainLayout->addWidget(inputField);

        QGridLayout *gridLayout = new QGridLayout();

        const QString buttons[5][4] = {
            {"C", "(", ")", "/"},
            {"7", "8", "9", "*"},
            {"4", "5", "6", "-"},
            {"1", "2", "3", "+"},
            {"0", ".", "Back", "="}
        };

        for (int row = 0; row < 5; ++row) {
            for (int col = 0; col < 4; ++col) {
                QString text = buttons[row][col];
                QPushButton *btn = new QPushButton(text);
                btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                btn->setStyleSheet("font-size: 18px; font-weight: 500; min-height: 50px;");
                gridLayout->addWidget(btn, row, col);

                if (text == "C") {
                    connect(btn, &QPushButton::clicked, this, &Calculator::handleClear);
                } else if (text == "=") {
                    connect(btn, &QPushButton::clicked, this, &Calculator::handleEquals);
                } else if (text == "Back") {
                    connect(btn, &QPushButton::clicked, this, &Calculator::handleBackspace);
                } else {
                    connect(btn, &QPushButton::clicked, this, [this, text]() {
                        inputField->setText(inputField->text() + text);
                    });
                }
            }
        }

        mainLayout->addLayout(gridLayout);
    }

private slots:
    void handleClear() { inputField->clear(); }

    void handleBackspace() {
        QString text = inputField->text();
        if (!text.isEmpty()) {
            text.chop(1);
            inputField->setText(text);
        }
    }

    void handleEquals() {
        QString expression = inputField->text();
        if (expression.isEmpty()) return;

        try {
            double result = MathParser::evaluate(expression);
            inputField->setText(QString::number(result));
        } catch (const std::exception& e) {
            inputField->setText("Syntax Error");
        }
    }

private:
    QLineEdit *inputField;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Calculator calculator;
    calculator.show();
    return app.exec();
}

#include "main.moc"