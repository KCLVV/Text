#include <SFML/Graphics.hpp>
#include <time.h>
#include<iostream>
#include<conio.h>

const int BodyMaxLong = 2000;        //�����
const int SnakeInitLongth = 10;      //��ʼ����
const sf::Vector2f SnakeInitPos = { 200,200 }; //��ͷ��ʼλ��
const int BodyRadius = 5;             //����뾶
const sf::Vector2i WindowSize = { 500,500 };
const int FoodRadius = 2;
const int FoodNum = 30;
#define SnakeColor sf::Color::Green
#define FoodColor sf::Color::Red

class Snake {                       //��
private:
    int longth;                            //������
    sf::Vector2f dir;                        //�ߵ�ǰ�н�����
    sf::CircleShape body[BodyMaxLong];        //��Բ��������������
public:
    Snake() {
        longth = SnakeInitLongth;
        dir = { 5 , 0 };                       //��ʼ����
        for (int i = 0; i < 5; i++) {            //��ʼ��λ�ú�����ÿ��Բ��֮��ļ��
            sf::Vector2f pos = { SnakeInitPos.x - i * 5, SnakeInitPos.y };
            body[i].setPosition(pos);
        }
        for (int i = 0; i < BodyMaxLong; i++) {        //��ʼ������뾶����ɫ
            body[i].setRadius(BodyRadius);
            body[i].setFillColor(SnakeColor);
        }
    }
    void SnakeMove() {                      //�����ƶ�������,��һ��ѭ�������ƶ�ÿһ������
        for (int i = longth - 1; i > 0; i--)
        {
            body[i].setPosition(body[i - 1].getPosition().x, body[i - 1].getPosition().y);
        }
        body[0].move(dir);
    }
    void SnakeHolding(sf::Event& event) {            //�������н�����
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
    sf::CircleShape PreDrawSnake(int i) {        //��������������ʱ��Ҫ����ÿ��Բ�ζ���
        return this->body[i];
    }

    int GetSnakeLongth() {
        return this->longth;
    }
    void UpLevel() { this->longth++; }            //�Ե�ʳ����������������
    sf::Vector2f GetDir() { return dir; }
};

typedef struct {        //һ��������x,y�������͵Ľṹ�壬���ڱ�ʾ���꣬����������vector2i�ģ������������Ǳ���ͳ����Լ�д���ṹ�壬���ֿ������о���ô������
    int x;
    int y;
}PosData;

class Food {                            //ʳ����
private:
    PosData pos;
    sf::CircleShape Body;
public:
    Food() {
        pos = { rand() % 500,rand() % 500 };            //��ʼ�����ʳ��λ��
        Body.setFillColor(FoodColor);                    //��ʼ��ʳ����ɫ
        Body.setPosition({ (float)pos.x,(float)pos.y });        //��λ�����ý�ͼ�ζ�����
        Body.setRadius(FoodRadius);                        //��ʼ��ʳ���ͼ�ΰ뾶
    }
    sf::Vector2f GetFoodPos() {                           //����ʳ��λ�õĺ��� 
        return { (float)this->pos.x + FoodRadius,(float)this->pos.y + FoodRadius };
    }
    void SetFoodPos() {                    //һ�����Ե�������������һ��λ��
        pos = { rand() % WindowSize.x, rand() % WindowSize.y };
        Body.setPosition({ (float)pos.x,(float)pos.y });
    }
    sf::CircleShape PreDraFood() { return this->Body; }        //����ͼ�ζ����Ա���ƺ���������
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
    font.loadFromFile("arial.ttf");            //���������ļ�
    cnfont.loadFromFile("hpsimplifiedhans-light.ttf");
    text.setPosition(1, 1);        //���������λ��
    score.setPosition(100, 1);
    pretext.setPosition(20, 150);
    pretext.setString("Welcome to shushuSnake Game\npress space button\n start the game");                        //�����ַ�����
    pretext.setFont(font);
    pretext.setFillColor(sf::Color::Blue);
    text.setString("Score:");
    text.setFont(font); score.setFont(font);
    text.setFillColor(sf::Color::Black); score.setFillColor(sf::Color::Black);
    std::string str;
    srand((int)time(0));
    sf::RenderWindow window(sf::VideoMode(WindowSize.x, WindowSize.y), "shushuSnake");
    Snake snake;            //������
    Food food[FoodNum];        //����ʳ��
    sf::Clock clock;                //
    double timer = 0, delay = 0.05;           //
    bool prep = 0;            //��prep = 0 �ȴ���Ұ��ո�ʼ��Ϸ����ʼ��Ϸ��prep = 1
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
            for (int i = 0; i < FoodNum; i++)                     //ʳ�����
            {
                window.draw(food[i].PreDraFood());
            }
            if (timer > delay) {
                timer = 0; snake.SnakeMove();
                for (int i = 0; i < FoodNum; i++)
                {
                    GetFoodCheck(snake, food[i]);
                }
                if (CollideCheck(snake) == 0)        //ҧ��β�ͻ���ײ��ǽʧ����
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
                    for (int i = 0; i < longth; i++)                     //�������
                        window.draw(snake.PreDrawSnake(i));
                    window.display();
                    if (_getch())return 0;
                }
                str = std::to_string(snake.GetSnakeLongth() - SnakeInitLongth);
                score.setString(str);
            }
            for (int i = 0; i < longth; i++)                     //�������
                window.draw(snake.PreDrawSnake(i));
            window.draw(text);
            window.draw(score);
            window.display();
        }

    }

    return 0;
}