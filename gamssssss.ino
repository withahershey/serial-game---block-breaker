
#define leftButton 25
#define rightButton 32
 
int score = 0;

const int width = 16;
const int height = 16;

int paddlex = 7;
const int paddleWidth = 3;
const int paddley = height -1 ;

int ballx = 8;
int bally = 8;
int ball_dir_x = 1;
int ball_dir_y = -1;

const int rows = 3;
const int col = 5;
bool gameRunning = true;

bool bricks[rows][col]; // for a grid structure

void drawGame(){
  for(int y=0; y<height; y++){
    for(int x=0; x<width; x++ ){
      //make bricks
      bool printed = false;
      
      // make the horizontal of the brick
      for(int r=0; r<rows; r++){
        
        // make the vertical of the bricks
        for(int c=0; c<col; c++){
          if(bricks[r][c] && x == c*3 && y== r){
            Serial.print("#");
            printed = true;
          }
        }
      }
      //make paddle
      if(y == paddley && x >= paddlex && x < paddlex + paddleWidth){
        Serial.print("=");
        printed = true;
      }

      //draw ballssss
      if(x ==ballx && y == bally){
        Serial.print("O");
        printed = true;
      }
      if(!printed) Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println("----------------");
}

void resetGame() {
  paddlex = 7;

  ballx = 8;
  bally = 8;
  ball_dir_x = 1;
  ball_dir_y = -1;

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < col; c++) {
      bricks[r][c] = true;
    }
  }

  score = 0;
  gameRunning = true;
}

void showScoreScreen() {
  Serial.print("\033[2J\033[H"); // clear terminal
  Serial.println("===== ROUND OVER =====");
  Serial.print("Your score: ");
  Serial.println(score);
  Serial.println();
  Serial.println("Hold BOTH buttons to restart...");
}

void setup() {
  // put your setup code here, to run once:
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);

  // Initialize bricks
  for(int r=0; r<rows; r++) {
    for(int c=0; c<col; c++) {
      bricks[r][c] = true; // all bricks active
    }
  }

  Serial.begin(115200);
  resetGame();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!gameRunning) return; // makes the game wait if u ded
  if(digitalRead(leftButton) == LOW && paddlex > 0) paddlex--;
  if(digitalRead(rightButton)== LOW && paddlex < width - paddleWidth) paddlex++;
  ballx += ball_dir_x;
  bally += ball_dir_y;

  if(ballx <= 0 || ballx >= width-1){
    ball_dir_x = -ball_dir_x;
  }
  if(bally <= 0){ 
    ball_dir_y = -ball_dir_y;
  }

  if(bally == paddley-1 && ballx >= paddlex && ballx < paddlex + paddleWidth){
    ball_dir_y = -ball_dir_y;
  }

  for(int r=0; r<rows; r++) {
    for(int c=0; c<col; c++) {
      if(bricks[r][c] && ballx == c*3 && bally == r) {
        bricks[r][c] = false;
        ball_dir_y = -ball_dir_y;
        score ++;
      }
    }
  }
  if (bally >= height) {
  gameRunning = false;
  showScoreScreen();

  // wait until BOTH buttons are held
  while (!(digitalRead(leftButton) == LOW && digitalRead(rightButton) == LOW)) {
    delay(50);
  }

  delay(300); // debounce / prevents instant restart spam
  resetGame();
  } 

  drawGame();
  delay(100); 

}
