#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <fstream>
#include <deque>
#include <vector>

using namespace std;

Color green = Color{96, 180, 204, 255};    // Soft teal blue  
Color darkGreen = Color{24, 51, 63, 255};  // Deep ocean blue  

int cellSize=30;
int cellCount=25;
int offset=75;

double lastUpdateTime=0;

bool eventTriggered(double interval){
    double CurrentTime=GetTime();
    if(CurrentTime-lastUpdateTime>=interval){
        lastUpdateTime=CurrentTime;
        return true;
    }
    return false;
}

bool ElementInDeque(Vector2 ele, deque<Vector2> q){
    for(int i=0;i<q.size();i++){
        if(Vector2Equals(q[i],ele)){
            return true;
        }
    }
    return false;
}

class Food{
    public:
        Vector2 position;
        Texture2D texture;

        Food(deque<Vector2> snakebody){
            Image image=LoadImage("assets/food.png");
            texture=LoadTextureFromImage(image);
            UnloadImage(image);
            position=GetRandomPos(snakebody);
        }

        void Draw(){
            DrawTexture(texture,offset+position.x*cellSize,offset+position.y*cellSize,WHITE);
        }
        
        Vector2 GetRandomCell(){
            float x=GetRandomValue(1,cellCount-2);
            float y=GetRandomValue(1,cellCount-2);
            return Vector2{x,y};
        }

        Vector2 GetRandomPos(deque<Vector2> snakebody){
            Vector2 position=GetRandomCell();
            while(ElementInDeque(position,snakebody)){
                position=GetRandomCell();
            }
            return position;
            
        }
        
};

class Snake{
    public:
        deque<Vector2> body={Vector2{6,9},Vector2{5,9},Vector2{4,9}};
        Vector2 direction={1,0};
        bool addSegment=false;

        void Draw(){
            for(unsigned int i=0;i<body.size();i++){
                int x=body[i].x;
                int y=body[i].y;
                Rectangle rect={(float)offset+x*cellSize,(float)offset+y*cellSize,(float)cellSize,(float)cellSize};
                DrawRectangleRounded(rect,0.5,6,darkGreen);
            }
        }

        void Update(){
            body.push_front(Vector2Add(body[0],direction));
            if(addSegment){
                addSegment=false;
            }
            else{
                body.pop_back();
            }
        }


        void Reset(){
            body={Vector2{6,9},Vector2{5,9},Vector2{4,9}};
            direction={1,0};
        }
};

class Game{
    public:
        Snake snake=Snake();
        vector<Food> foods; // Store multiple food instances
        int maxFood = 3;    // Maximum food items at a time
        bool running=false;
        double startTime=0;
        double pauseTime=0;
        double countdown = 10.0;
        int minutes,seconds;

        int score=0;
        int highscore=0;
        Sound eatSound;
        Sound wallSound;

        Game(){
            InitAudioDevice();
            eatSound=LoadSound("assets/eat.mp3");
            wallSound=LoadSound("assets/wall.mp3");

            LoadHighScore();
            startTime=GetTime();

            // Generate initial food items
            for (int i = 0; i < maxFood; i++) {
                foods.push_back(Food(snake.body));
            }
        }

        ~Game(){
            CloseAudioDevice();
            UnloadSound(eatSound);
            UnloadSound(wallSound);
        }

        void Draw(){
            // Draw all food items
            for (Food &food : foods) {
                food.Draw();
            }
            snake.Draw();

        }

        void Update(){
            if(running){
                // Calculate remaining time
                double elapsedTime = GetTime() - startTime - pauseTime;
                double timeLeft = countdown - elapsedTime;
        
                // Clamp time to avoid negative values
                if (timeLeft < 0) timeLeft = 0;
        
                minutes = (int)timeLeft / 60;
                seconds = (int)timeLeft % 60;
        
                // Stop the game when timer reaches 0
                if (elapsedTime >= countdown) {
                    GameOver();
                    return;
                }
        
                snake.Update();
                CheckCollisionWithFood();
                CheckCollisionWithEdge();
                CheckCollisionWithBody();
            }
        }

        void CheckCollisionWithFood(){
            for (int i = 0; i < foods.size(); i++) {
                if (Vector2Equals(snake.body[0], foods[i].position)) {
                    // Remove eaten food and spawn new one
                    foods[i] = Food(snake.body);
                    snake.addSegment = true;
                    PlaySound(eatSound);
                    score++;
    
                    // Increase countdown by 2 seconds
                    countdown += 2.0;
                }
            }
        }

        void CheckCollisionWithEdge(){
            if(snake.body[0].x==cellCount||snake.body[0].x==-1){
                GameOver();
            }
            if(snake.body[0].y==cellCount||snake.body[0].y==-1){
                GameOver();
            }
        }

        void CheckCollisionWithBody(){
            deque<Vector2> headlessBody=snake.body;
            headlessBody.pop_front();
            if(ElementInDeque(snake.body[0],headlessBody)){
                GameOver();
            }
        }

        void StartGame(){
            running=true;
            startTime=GetTime();
            pauseTime=0;
        }

        void GameOver(){
            snake.Reset();
            foods.clear();
            // Respawn food items
            for (int i = 0; i < maxFood; i++) {
                foods.push_back(Food(snake.body));
            }
            running=false;
            PlaySound(wallSound);
            UpdateHighScore();
            
            // Reset countdown to 10 seconds before restarting
            countdown = 10.0;
            startTime = GetTime(); // Reset reference time
            pauseTime = 0;
            score = 0;
        }

        void LoadHighScore(){
            std::ifstream file("highscore.txt");
            if (file.is_open()) {
                file >> highscore;
                file.close();
            }
        }   

        void ResetHighScore(){
            std::ofstream file("highscore.txt");
                if (file.is_open()) {
                    file << 0;
                    file.close();
                }
        }

        void UpdateHighScore(){
            if(score>highscore){
                highscore=score;
                std::ofstream file("highscore.txt");
                if (file.is_open()) {
                    file << highscore;
                    file.close();
                }
            }
        }
};

int main () {
    InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Snake Eater");
    SetTargetFPS(60);
    
    Game game = Game();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(green);

        // Only update the game logic if running
        if (eventTriggered(0.1) && game.running) {
            game.Update();
        }

        // Handle movement and starting the game
        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
            game.snake.direction = {0, -1};
            if (!game.running) game.StartGame();
        }
        else if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
            game.snake.direction = {0, 1};
            if (!game.running) game.StartGame();
        }
        else if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
            game.snake.direction = {-1, 0};
            if (!game.running) game.StartGame();
        }
        else if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
            game.snake.direction = {1, 0};
            if (!game.running) game.StartGame();
        }

        // Display the timer and game status
        if (!game.running) {
            DrawText("00:10", offset + cellSize * 12, 20, 40, darkGreen);

            const char* startText = "Press any arrow key to start";
            int textWidth = MeasureText(startText, 40);  // Get width of text in pixels
            int textHeight = 40;  // Font size is 20, so height is roughly 20px
            int centerX = (GetScreenWidth() - textWidth) / 2;  // Center horizontally
            int centerY = (GetScreenHeight() - textHeight) / 2;  // Center vertically
            DrawText(startText, centerX, centerY, 40, darkGreen);

        } else {
            DrawText(TextFormat("%02d:%02d", game.minutes, game.seconds), offset + cellSize * 12, 20, 40, darkGreen);
        }

        game.Draw();

        // Draw UI elements
        DrawRectangleLinesEx(Rectangle{
            (float)offset-5,
            (float)offset-5,
            (float)cellSize * cellCount + 10,
            (float)cellSize * cellCount + 10,
        }, 5, darkGreen);
        DrawText("Snake Eater", offset - 5, 20, 40, darkGreen);
        DrawText(TextFormat("%d", game.score), offset + cellSize * cellCount - 15, 20, 40, darkGreen);
        DrawText(TextFormat("High Score: %d", game.highscore), offset - 5, offset + cellSize * cellCount + 10, 30, darkGreen);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
