#include <SFML/Graphics.hpp>
#include <time.h>
#include<iostream>
#include<conio.h>

const int BodyMaxLong = 2000;        //最长蛇身
const int SnakeInitLongth = 10;      //初始蛇身
const sf::Vector2f SnakeInitPos = { 200,200 }; //蛇头初始位置
const int BodyRadius = 5;             //蛇身半径
const sf::Vector2i WindowSize = { 500,500 };
const int FoodRadius = 2;
const int FoodNum = 30;
#define SnakeColor sf::Color::Green
#define FoodColor sf::Color::Red

class Snake {                       //蛇
private:
    int longth;                            //蛇身长度
    sf::Vector2f dir;                        //蛇当前行进方向
    sf::CircleShape body[BodyMaxLong];        //用圆形类来建造蛇身
public:
    Snake() {
        longth = SnakeInitLongth;
        dir = { 5 , 0 };                       //初始方向
        for (int i = 0; i < 5; i++) {            //初始化位置和蛇身每个圆形之间的间距
            sf::Vector2f pos = { SnakeInitPos.x - i * 5, SnakeInitPos.y };
            body[i].setPosition(pos);
        }
        for (int i = 0; i < BodyMaxLong; i++) {        //初始化蛇身半径和颜色
            body[i].setRadius(BodyRadius);
            body[i].setFillColor(SnakeColor);
        }
    }
    void SnakeMove() {                      //整体移动蛇身函数,用一个循环依次移动每一个蛇身
        for (int i = longth - 1; i > 0; i--)
        {
            body[i].setPosition(body[i - 1].getPosition().x, body[i - 1].getPosition().y);
        }
        body[0].move(dir);
    }
    void SnakeHolding(sf::Event& event) {            //控制蛇行进方向
        switch (event.key.code)
        {
        case sf::Keyboard::Up:
        case sf::Keyboard::W:if (dir.x != 0 && dir.y != 5) { dir = { 0,-5 }; }break;
        case sf::Keyboard::Left:
        case sf::Keyboard::A:if (dir.x != 5 && dir.y != 0) { dir = { -5,0 }; }break;
        case sf::Keyboard::Right:
        case sf::Keyboard::D:if (dir.x != -5 && dir.y != 0) { dir = { 5,0 }; }break;
        case sf::Keyboard::Down:
        case sf::Keyboard::S:if (dir.x != 0 && dir.y != -5) { dir = { 0 , 5 }; }break;
        default:
            break;
        }
    }
    sf::CircleShape PreDrawSnake(int i) {        //在主函数绘制蛇时需要返回每个圆形对象
        return this->body[i];
    }

    int GetSnakeLongth() {
        return this->longth;
    }
    void UpLevel() { this->longth++; }            //吃到食物升级，长度自增
    sf::Vector2f GetDir() { return dir; }
};

typedef struct {        //一个包含下x,y两个整型的结构体，用于表示坐标，本来可以用vector2i的，发现下面老是报错就尝试自己写个结构体，发现可以运行就这么运行了
    int x;
    int y;
}PosData;

class Food {                            //食物类
private:
    PosData pos;
    sf::CircleShape Body;
public:
    Food() {
        pos = { rand() % 500,rand() % 500 };            //初始化随机食物位置
        Body.setFillColor(FoodColor);                    //初始化食物颜色
        Body.setPosition({ (float)pos.x,(float)pos.y });        //将位置设置进图形对象中
        Body.setRadius(FoodRadius);                        //初始化食物的图形半径
    }
    sf::Vector2f GetFoodPos() {                           //返回食物位置的函数 
        return { (float)this->pos.x + FoodRadius,(float)this->pos.y + FoodRadius };
    }
    void SetFoodPos() {                    //一旦被吃掉，重新随机获得一个位置
        pos = { rand() % WindowSize.x, rand() % WindowSize.y };
        Body.setPosition({ (float)pos.x,(float)pos.y });
    }
    sf::CircleShape PreDraFood() { return this->Body; }        //返回图形对象，以便绘制和其他功能
};


void GetFoodCheck(Snake& snake, Food& food) {
    sf::Vector2f snakepos = { snake.PreDrawSnake(0).getPosition().x + BodyRadius,snake.PreDrawSnake(0).getPosition().y + BodyRadius };
    sf::Vector2f foodpos = food.GetFoodPos();
    if (foodpos.x >= snakepos.x - BodyRadius && foodpos.x <= snakepos.x + BodyRadius && foodpos.y >= snakepos.y - BodyRadius && foodpos.y <= snakepos.y + BodyRadius) {
        snake.UpLevel();
        food.SetFoodPos();
    }
}

bool CollideCheck(Snake& snake) {
    sf::CircleShape head = snake.PreDrawSnake(0);
    if (head.getPosition().x >= WindowSize.x || head.getPosition().x <= 0 || head.getPosition().y >= WindowSize.y || head.getPosition().y <= 0)
    {
        return 0;
    }
    int longth = snake.GetSnakeLongth();
    for (int i = 1; i < longth; i++) {
        if (snake.GetDir().x == 0)
        {
            if (snake.PreDrawSnake(0).getPosition().x + BodyRadius >= snake.PreDrawSnake(i).getPosition().x && snake.PreDrawSnake(0).getPosition().x + BodyRadius <= snake.PreDrawSnake(i).getPosition().x + BodyRadius * 2)
            {
                if (snake.GetDir().y > 0)
                {
                    if (snake.PreDrawSnake(0).getPosition().y + BodyRadius * 2 >= snake.PreDrawSnake(i).getPosition().y && snake.PreDrawSnake(0).getPosition().y + BodyRadius * 2 <= snake.PreDrawSnake(i).getPosition().y + BodyRadius * 2)
                    {
                        return 0;
                    }
                }
                else {
                    if (snake.PreDrawSnake(0).getPosition().y >= snake.PreDrawSnake(i).getPosition().y && snake.PreDrawSnake(0).getPosition().y <= snake.PreDrawSnake(i).getPosition().y + BodyRadius * 2)
                    {
                        return 0;
                    }
                }
            }
        }
        if (snake.GetDir().y == 0)
        {
            if (snake.PreDrawSnake(0).getPosition().y + BodyRadius >= snake.PreDrawSnake(i).getPosition().y && snake.PreDrawSnake(0).getPosition().y + BodyRadius <= snake.PreDrawSnake(i).getPosition().y + BodyRadius * 2)
            {
                if (snake.GetDir().x > 0)
                {
                    if (snake.PreDrawSnake(0).getPosition().x + BodyRadius * 2 >= snake.PreDrawSnake(i).getPosition().x && snake.PreDrawSnake(0).getPosition().x + BodyRadius * 2 <= snake.PreDrawSnake(i).getPosition().x + BodyRadius * 2)
                    {
                        return 0;
                    }
                }
                else {
                    if (snake.PreDrawSnake(0).getPosition().x >= snake.PreDrawSnake(i).getPosition().x && snake.PreDrawSnake(0).getPosition().x <= snake.PreDrawSnake(i).getPosition().x + BodyRadius * 2)
                    {
                        return 0;
                    }
                }
            }
        }

    }
    return 1;
}

int main()
{
    sf::Text text, score, pretext;
    sf::Font font, cnfont;
    font.loadFromFile("arial.ttf");            //加载字体文件
    cnfont.loadFromFile("hpsimplifiedhans-light.ttf");
    text.setPosition(1, 1);        //设置字体的位置
    score.setPosition(100, 1);
    pretext.setPosition(20, 150);
    pretext.setString("Welcome to shushuSnake Game\npress space button\n start the game");                        //设置字符内容
    pretext.setFont(font);
    pretext.setFillColor(sf::Color::Blue);
    text.setString("Score:");
    text.setFont(font); score.setFont(font);
    text.setFillColor(sf::Color::Black); score.setFillColor(sf::Color::Black);
    std::string str;
    srand((int)time(0));
    sf::RenderWindow window(sf::VideoMode(WindowSize.x, WindowSize.y), "shushuSnake");
    Snake snake;            //创建蛇
    Food food[FoodNum];        //创建食物
    sf::Clock clock;                //
    double timer = 0, delay = 0.05;           //
    bool prep = 0;            //若prep = 0 等待玩家按空格开始游戏，开始游戏后prep = 1
    int longth;
    while (window.isOpen())
    {
        if (!prep)
        {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Space)
                    {
                        prep = 1;
                    }
                }
            }
            window.clear(sf::Color::White);
            window.draw(text);
            score.setString("0");
            window.draw(score);
            window.draw(pretext);
            window.display();
        }
        if (prep) {
            float time = clock.getElapsedTime().asSeconds();        //
            clock.restart();                        //
            timer += time;
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    snake.SnakeHolding(event);
                }

            }
            longth = snake.GetSnakeLongth();
            window.clear(sf::Color::White);
            for (int i = 0; i < FoodNum; i++)                     //食物绘制
            {
                window.draw(food[i].PreDraFood());
            }
            if (timer > delay) {
                timer = 0; snake.SnakeMove();
                for (int i = 0; i < FoodNum; i++)
                {
                    GetFoodCheck(snake, food[i]);
                }
                if (CollideCheck(snake) == 0)        //咬到尾巴或者撞到墙失败了
                {
                    sf::Text endtext;
                    endtext.setString("Game Over!!");
                    endtext.setPosition(100, 150);
                    endtext.setFont(font);
                    endtext.setCharacterSize(50);
                    endtext.setFillColor(sf::Color::Red);
                    window.clear(sf::Color::White);
                    window.draw(text);
                    window.draw(score);
                    window.draw(endtext);
                    longth = snake.GetSnakeLongth();
                    for (int i = 0; i < longth; i++)                     //蛇身绘制
                        window.draw(snake.PreDrawSnake(i));
                    window.display();
                    if (_getch())return 0;
                }
                str = std::to_string(snake.GetSnakeLongth() - SnakeInitLongth);
                score.setString(str);
            }
            for (int i = 0; i < longth; i++)                     //蛇身绘制
                window.draw(snake.PreDrawSnake(i));
            window.draw(text);
            window.draw(score);
            window.display();
        }

    }

    return 0;
}