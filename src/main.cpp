#include <iostream>
#include <raylib.h>
#include <ctime>
#include <string>

using namespace std;

Color fundo = {30,30,30,255};//{30,30,30,255}
Color blocks = {20,20,20,255};
Color blocksRed = {200,100,100,255};
Color blocksGreen = {100,200,100,255};
Color border = {100,100,200,255};

int numeros[7] = {2,3,7,5,3,7,2};

void gera(int array[]){
    int count = 0;
    bool foi = false;
    int aleatorio = 0;

    for(int i=0;i<16;i++){
        array[i]=0;
    }

    while(!foi){
        count = 0;
        for(int i=0;(i<16) && (count<4);i++){
            aleatorio = (rand() % 100);
            int divisivelPor = (rand() % 6);
            if(aleatorio%numeros[divisivelPor] == 0){
                count++;
                array[i] = 1;
            }
        }

        int pares = 0;

        for(int i=0;i<16;i++){
            if(array[i]==1){
                pares++;
            }
            if(pares==4){
                foi = true;
            }
        }
    }

    foi = false;
    count = 0;

    while(!foi){
        count = 0;

        for(int i=0;(i<16) && (count<6);i++){
            aleatorio = (rand() % 100);
            int divisivelPor = (rand() % 6);
            if(aleatorio%numeros[divisivelPor] == 0){
                count++;
                if(array[i]==1){
                    int point = i;
                    while(array[i]==1){
                        i++;
                        if(array[i]==0){
                            array[i]=2;
                        }
                    }
                    i=point;
                }else{
                    array[i] = 2;
                }
            }
        }

        int pares = 0;

        for(int i=0;i<16;i++){
            if(array[i]==2){
                pares++;
            }
            if(pares==6){
                foi = true;
            }
        }

    }

    for(int i=0;i<16;i++)
        cout<<array[i]<<" ";
    cout<<endl;
}

void aumentaSlaOq(int qnt,float* oq,Vector2 pos){
    int mouseX = GetMouseX();
    int mouseY = GetMouseY();


    int width = 80;
    int spaces = 10;
    Rectangle mais = {(float)pos.x,(float)pos.y,(float)width,(float)width};
    Rectangle numero = {(float)pos.x,(float)pos.y+width+spaces,(float)width,(float)width};
    Rectangle menos = {(float)pos.x,(float)pos.y+spaces*2+width*2,(float)width,(float)width};

    DrawRectangleRounded(mais,0.2f,4,blocks);
    DrawRectangleRounded(numero,0.2f,4,blocks);
    DrawRectangleRounded(menos,0.2f,4,blocks);

    
    DrawText(TextFormat("%d",qnt),(numero.x+width/2)-20,(numero.y+width/2)-20,40,WHITE);
    DrawText("+",(mais.x+width/2)-10,(mais.y+width/2)-20,40,WHITE);
    DrawText("-",(menos.x+width/2)-10,(menos.y+width/2)-20,40,WHITE);


    if(CheckCollisionRecs(mais,{(float)mouseX,(float)mouseY,5,5})){
        DrawRectangleRoundedLines(mais,0.2f,4,4,blocksGreen);
        if(IsMouseButtonPressed(0)){
            *oq = *oq + qnt;
        }

    }else if(CheckCollisionRecs(menos,{(float)mouseX,(float)mouseY,5,5})){
        DrawRectangleRoundedLines(menos,0.2f,4,4,blocksRed);
        if(IsMouseButtonPressed(0)){
            *oq = *oq - qnt;
        }
    }


}

int main(){
    //inicia com uma semente baseada no tempo
    srand(static_cast<unsigned int>(time(NULL)));

    const int ScreenWidth = 1000;
    const int ScreenHeight = 800;

    int mouseX = 0;
    int mouseY = 0;

    int cont = 0;

    int array[16] = {0}; //array contendo onde tem bomba e onde não tem
    int array2[16] = {0}; //array onde tem os blocos clicados

    int momento = 0; //momento(0-inicio 1-jogo 2-ganhos)

    int diamantes = 0; //diamantes encontrados
    int bombas = 0; //bombas encontradas
    
    float animacaoDinDin = 0.f; //usado pra fzr a "animação" do dinheiro subindo ou descendo
    float dinheiro = 0.f;
    float dinheiroFinal = 0.f;

    gera(array);

    InitWindow(ScreenWidth,ScreenHeight,"Diamond Finder");
    SetTargetFPS(60);
    SetExitKey(0);

    //carrega os arquivos na CPU para poderem ser editadas
    Image bombaI = LoadImage("imgs/1.png");
    Image diamanteI = LoadImage("imgs/0.png");

    //triplica o tamanho das imagens
    ImageResize(&bombaI,bombaI.width*3,bombaI.height*3);
    ImageResize(&diamanteI,diamanteI.width*3,diamanteI.height*3);

    //carrega as imagens modificadas em texturas
    Texture2D bomba = LoadTextureFromImage(bombaI);
    Texture2D diamante = LoadTextureFromImage(diamanteI);

    //libera memoria
    UnloadImage(bombaI);
    UnloadImage(diamanteI);

    double timer1 = 0; //tempo depois de ter concluido todos os blocos

    while(WindowShouldClose() == false){

        mouseX = GetMouseX();
        mouseY = GetMouseY();

        BeginDrawing();
        ClearBackground(fundo);

        if (momento == 1){
            //checa se tá tudo cheio
            for(int i=0;(i<16) && (cont<16);i++){
                if(array2[i] == 1){
                    cont++;
                }
            }

            //limpa todo o array de clicados (array2)
            if(cont==16){
                if(timer1==0){
                    timer1 = GetTime();
                }
                if(3<GetTime()-timer1){
                    gera(array);
                    for(int i=0;i<16;i++){
                        array2[i]=0;
                    }
                    timer1=0;
                }
            }
            cont=0;
        
            for(int i=0;i<4;i++){
                for(int j=0;j<4;j++){
                    Rectangle rec = {(float)130+(200*j),(float)20+(170*i),150,150};
                    

                    if(array2[(i*4)+j]==1){
                        if(array[(i*4)+j]==1){
                            DrawRectangleRounded(rec,0.2f,4,blocksGreen);
                            DrawTexture(diamante,(rec.x+(rec.width/2))-(diamante.width/2),(rec.y+(rec.height/2))-(diamante.height/2),WHITE);
                        }else if(array[(i*4)+j]==0){
                            DrawRectangleRounded(rec,0.2f,4,blocksRed);
                            DrawTexture(bomba,(rec.x+(rec.width/2))-(bomba.width/2),(rec.y+(rec.height/2))-(bomba.height/2),WHITE);
                        }else if(array[(i*4)+j]==2){
                            DrawRectangleRounded(rec,0.2f,4,border);
                        }
                    }else{
                        DrawRectangleRounded(rec,0.2f,4,blocks);
                    }

                    if(CheckCollisionRecs(rec,{(float)mouseX,(float)mouseY,5,5})){
                        DrawRectangleRoundedLines(rec,0.2f,4,4,border);
                        if(IsMouseButtonPressed(0) && array2[(i*4)+j]==0){
                            array2[(i*4)+j] = 1;
                            if(array[(i*4)+j]==1){
                                diamantes ++;
                            }else if(array[(i*4)+j]==0){
                                bombas ++;
                            }
                        }
                    }
                }
            }

            Rectangle btnFim = {770,720,200,50};
            DrawRectangleRounded(btnFim,0.2f,4,blocks);
            DrawText("Fim",btnFim.x+70,btnFim.y+5,40,WHITE);
            if(CheckCollisionRecs(btnFim,{(float)mouseX,(float)mouseY,5,5})){
                DrawRectangleRoundedLines(btnFim,0.2f,4,4,blocksRed);
                if(IsMouseButtonPressed(0)){
                    dinheiroFinal = dinheiro;
                    momento = 2;
                }
            }

            Rectangle btnDin = {30,720,200,50};
            DrawRectangleRounded(btnDin,0.2f,4,blocks);
            char dindin[20];
            sprintf(dindin, "R$ %.2f", dinheiro);
            DrawText(dindin,btnDin.x+20,btnDin.y+5,40,blocksGreen);
            DrawRectangleRoundedLines(btnDin,0.2f,4,4,blocksGreen);
            

        }else if(momento == 0){
            Vector2 posIni = {130,300};
            aumentaSlaOq(10,&dinheiro,{posIni.x,posIni.y});
            aumentaSlaOq(5,&dinheiro,{posIni.x+90,posIni.y});
            aumentaSlaOq(1,&dinheiro,{posIni.x+190,posIni.y});

            char texto[20];
            sprintf(texto, "R$ %.2f", dinheiro);

            DrawRectangleRounded({posIni.x+280,posIni.y+90,200,80},0.2f,4,blocks);
            DrawRectangleRoundedLines({posIni.x+280,posIni.y+90,200,80},0.2f,4,4,blocksGreen);
            DrawText(texto,posIni.x+290,posIni.y+110,40,WHITE);

            aumentaSlaOq(-10,&dinheiro,{posIni.x+490,posIni.y});
            aumentaSlaOq(-5,&dinheiro,{posIni.x+580,posIni.y});
            aumentaSlaOq(-1,&dinheiro,{posIni.x+670,posIni.y});

            Rectangle btnCon = {400,600,200,50};
            DrawRectangleRounded(btnCon,0.2f,4,blocks);

            DrawText("jogar",450,605,40,WHITE);

            if(CheckCollisionRecs(btnCon,{(float)mouseX,(float)mouseY,5,5})){
                DrawRectangleRoundedLines(btnCon,0.2f,4,4,border);
                if(IsMouseButtonPressed(0)){
                    momento = 1;
                }
            }

        }else if(momento == 2){

            DrawText(TextFormat("Diamantes achados:%d",diamantes),0,0,40,border);

            DrawText(TextFormat("Bombas achadas:%d",bombas),0,60,40,blocksRed);

            char dindinfinal[100];
            sprintf(dindinfinal,"Ganhos R$ %.2f",(float)(diamantes*dinheiroFinal));
            DrawText(dindinfinal,0,120,40,blocksGreen);

            sprintf(dindinfinal,"Perdas R$ %.2f",(float)(bombas*dinheiroFinal));
            DrawText(dindinfinal,0,180,40,blocksRed);

            DrawText(TextFormat("Dinheiro apostado:%f",dinheiroFinal),0,240,40,border);

            dinheiro = dinheiroFinal + (diamantes*dinheiroFinal) - (bombas*dinheiroFinal);

            DrawText(TextFormat("DinDin Final:%f",animacaoDinDin),0,300,40,WHITE);
            if(animacaoDinDin<dinheiro){
                animacaoDinDin += 1.f;
            }else if(animacaoDinDin>dinheiro){
                animacaoDinDin = dinheiro;
            }


        }

        EndDrawing();
        
    }

    UnloadTexture(bomba);
    UnloadTexture(diamante);
    CloseWindow();

    return 0;
}