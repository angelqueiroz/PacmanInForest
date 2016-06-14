#include <allegro.h>
#include <math.h>



void init();
void deinit();

int  menu();
void jogar();
void creditos();
//void creditos();

bool AtualizaTempoP(int tmilisegundos){
    static double tempopacman;
    static clock_t t0pac=clock(),  tfpac;
    tfpac=clock();
    tempopacman=  ( (double) (tfpac - t0pac) ) / (CLOCKS_PER_SEC/1000);
    if(tempopacman>tmilisegundos){
        tempopacman=0;
        t0pac=clock();
        return true;
    }
    return false;
}
bool AtualizaTempoPulo(int tmilisegundos){
    static double tempopacman;
    static clock_t t0pac=clock(),  tfpac;
    tfpac=clock();
    tempopacman=  ( (double) (tfpac - t0pac) ) / (CLOCKS_PER_SEC/1000);
    if(tempopacman>tmilisegundos){
        tempopacman=0;
        t0pac=clock();
        return true;
    }
    return false;
}

typedef struct{
    int x,y;
    bool ativo;
    int tam;
    BITMAP *tiros;
}TIRO;

typedef struct{

    float pos_pers_mundo; //10 mil de tamanho
    float pos_pers_tela;  //800 de tamanho
    int direcao;
    int direcaold;
    int i, ix, iy;
    BITMAP *person; //ponteiro de bit para pers
    int y;
    bool ativo;
}PERSONAGEM;
PERSONAGEM jogador;

typedef struct{
    float pos_pers_mundo; //10 mil de tamanho
    float pos_pers_tela;  //800 de tamanho
    int direcao;
    int direcaold;
    int i, ix, iy;
    BITMAP *inim; //ponteiro de bit para inimigo
    int y, x;
    bool ativo;
}INIMIGO;
INIMIGO inimigo;

typedef struct{
    int pos_sprite;
    int sprite_atual;
    int sprite_limite;
    BITMAP *sprites[3]; //ponteiro de bit para sprites
}SPRITES;


typedef struct{
    int x, y;
    int vel;
    int ativo;
    int direcaotiro;
    BITMAP *tiro;
}tiro;


enum{
    ATIVO,
    NAO_ATIVO
};

tiro fire[10];
void iniciar_tiros();
void show_tiros(tiro*fire, BITMAP *buffer);
void atirar();


int main() {
init();
int opcao=0;

while (opcao!=3) {
          opcao=menu();
          if(opcao==1)
               jogar();
          else if(opcao==2)
               creditos();
    }//fim do while

deinit();
return 0;
}
END_OF_MAIN();

int  menu(){
     BITMAP *buffer  = create_bitmap(800,400);
     BITMAP *botao1ns = load_bitmap("opcao1ns.bmp",NULL);
     BITMAP *botao1fs = load_bitmap("opcao1fs.bmp",NULL);
     BITMAP *botao2ns = load_bitmap("opcao2ns.bmp",NULL);
     BITMAP *botao2fs = load_bitmap("opcao2fs.bmp",NULL);
     BITMAP *botao3ns = load_bitmap("opcao3ns.bmp",NULL);
     BITMAP *botao3fs = load_bitmap("opcao3fs.bmp",NULL);
     BITMAP *fundo    = load_bitmap("fundo.bmp"   ,NULL);

     while(true){

          //renderiza o menu usando as imagens sem selecao
           blit(fundo, buffer, 0,0,0,0,800,400);
           masked_blit(botao1ns, buffer,0,0,160, 150,250,50);
           masked_blit(botao2ns, buffer,0,0,160, 200,250,50);
           masked_blit(botao3ns, buffer,0,0,160, 250,250,50);


          //produz o comportamento do mouse passando por cima
           if(mouse_x>160 && mouse_x<410 &&
              mouse_y>150 && mouse_y<200){
                  masked_blit(botao1fs, buffer,0,0,160, 150,250,50);
           }else if(mouse_x>160 && mouse_x<410 &&
              mouse_y>200 && mouse_y<250){
                  masked_blit(botao2fs, buffer,0,0,160, 200,250,50);
           }else if(mouse_x>160 && mouse_x<410 &&
              mouse_y>250 && mouse_y<300){
                  masked_blit(botao3fs, buffer,0,0,160, 250,250,50);
           }

          //se houver o click do mouse, retornar o valor
           if(mouse_b & 1){
                if(mouse_x>160 && mouse_x<410 &&
                   mouse_y>150 && mouse_y<200){
                         return 1;
                }else if(mouse_x>160 && mouse_x<410 &&
                   mouse_y>200 && mouse_y<250){
                         return 2;
                }else if(mouse_x>160 && mouse_x<410 &&
                   mouse_y>250 && mouse_y<300){
                         return 3;
               }
           }

           show_mouse(buffer);
           blit(buffer, screen, 0,0,0,0,800,400);

     }//fim do while


}//fim da funcao menu
void creditos(){
    BITMAP *buffer  = create_bitmap(800,400);
    BITMAP *fundo    = load_bitmap("fundo.bmp",NULL);
    blit(fundo, buffer, 0,0,0,0,800,400);
    blit(buffer, screen, 0,0,0,0,800,400);
    while(!key[KEY_ESC])
     {

    textprintf_ex( screen, font, 100, 10, makecol(224,24,24), -1,"|    FEMA - FUNDACAO EDUCACIONAL DO MUNICIPIO DE ASSIS      |");
    textprintf_ex( screen, font, 60, 20, makecol(80,208,17), -1,"     |        Curso de Bacharel em Ciencia da Computacao         |");
    textprintf_ex( screen, font, 60, 30, makecol(80,208,17), -1,"     | Projeto/Game . : Desenvolvimento de Jogo PacMan in forest |");
    textprintf_ex( screen, font, 60, 40, makecol(80,208,17), -1,"     | Desenvolvedores: Leonardo K. Z. Jubran - RA 1511420185    |");
    textprintf_ex( screen, font, 60, 50, makecol(80,208,17), -1,"     | Desenvolvedores: Angelica B. de O. Queiroz - RA 1511420069|");
    textprintf_ex( screen, font, 60, 60, makecol(80,208,17), -1,"     |                     Linguagem. . . : C++                  |");
    textprintf_ex( screen, font, 60, 350, makecol(237,227,39), -1,"                                      Pressione ESC para sair");

     }
};


void jogar(){
    for(int i=0; i<10; i++){
    fire[i].ativo=NAO_ATIVO;
}

PERSONAGEM jogador;
    jogador.pos_pers_mundo=100;
    jogador.pos_pers_tela=100;
    jogador.y=300;
    jogador.direcao=0;
    jogador.ativo=true;

    jogador.ix=30;
    jogador.iy=0;
    jogador.i=0;
    jogador.direcaold=0;

    INIMIGO inimigo;
    inimigo.pos_pers_mundo=100;
    inimigo.pos_pers_tela=100;
    inimigo.y=300;
    inimigo.direcao=0;
    inimigo.ativo=true;

    inimigo.ix=30;
    inimigo.iy=0;
    inimigo.i=0;
    inimigo.direcaold=0;



SPRITES tela;
    tela.pos_sprite=0;
    tela.sprite_atual=0;
    tela.sprite_limite=800;

    //abrir as imagens

tela.sprites[0]=load_bitmap("fundo11.bmp",NULL);
tela.sprites[1]=load_bitmap("fundo11.bmp",NULL);
tela.sprites[2]=load_bitmap("fundo11.bmp",NULL);

jogador.person=load_bitmap("pacman.bmp",NULL);
inimigo.inim=load_bitmap("pacman.bmp",NULL);
fire[0].tiro=load_bitmap("tiro.bmp",NULL);
BITMAP *box=load_bitmap("box.bmp",NULL);//142x79



    //double buffer
BITMAP *buffer=create_bitmap(800,400);

float contapulo, incremento=1;
bool PULAR=false;
//for(int i=0; fire[i]<10; i++){
  //  fire[i].ativo=false;


      while(!key[KEY_ESC])
     {
                 blit(box, buffer,0,0,300,200,60,60);

     if(key[KEY_UP] && !PULAR){
        PULAR=true;
        contapulo=-70; //tamanho do pulo;
    }

     if(PULAR && AtualizaTempoPulo(10)){  //pulo simples do personagem
        contapulo=contapulo+5;
        jogador.y+=(contapulo>0?5:-5);
        if(contapulo>=70)PULAR=false;
    }

//faz a movimentação do personagem pra frente e da sprite pra traz
    if(key[KEY_RIGHT]){
        jogador.direcao=1;
        jogador.pos_pers_mundo+=0.70; //move pers no mundo
        jogador.pos_pers_tela+=0.70;  //move pers na tela


             //limitador de mundo
             //                                                 \/-tamanho do mundo
             // if(jogador.pos_pers_mundo>10000){
             //       jogador.pos_pers_mundo--;
             //       jogador.pos_pers_tela--;
             //}

             //limitador de tela
             //                                           \/ - tamanho da tela de movimento do personagem
        if(jogador.pos_pers_tela>380){
            jogador.pos_pers_tela--; //recupera mov pers
            tela.pos_sprite--; //move tela
            if((tela.pos_sprite % 800)==0){ //se tela chegou ao limite
            //muda a sprite atual
            tela.sprite_atual=(tela.sprite_atual +1) % 3;
            tela.pos_sprite=0; //reseta a posicao
            }
        }
    }//fim if KEY_RUGHT

     if(key[KEY_LEFT]){
        jogador.direcao = -1;
        jogador.pos_pers_mundo-=0.70; //move pers no mundo
        jogador.pos_pers_tela-=0.70;  //move pers na tela

             //limitador de mundo
             //                                              \/ limitador de mundo lado esquerdo
             // if(jogador.pos_pers_mundo<100){
             //       jogador.pos_pers_mundo++;
             //       jogador.pos_pers_tela++;
             // }

             //limitador de tela
             //                                         \/-limitador de movimento personagem tela
        if(jogador.pos_pers_tela<100){
            jogador.pos_pers_tela++; //recupera mov pers
            tela.pos_sprite++; //move tela
            if(((tela.pos_sprite-1) % 800)==0){ //se tela chegou ao limite
            //muda a sprite atual
                tela.sprite_atual=((tela.sprite_atual-1)<0?2:(tela.sprite_atual-1));
                tela.pos_sprite=-799; //reseta a posicao
            }
        }
    }//fim KEY_LEFT
        if(key[KEY_SPACE] && AtualizaTempoP(100)){
        int i;
            for (i = 0; i < 10; i++){
                if (fire[i].ativo == NAO_ATIVO){
                    fire[i].ativo = ATIVO;
                    // Setamos a coordenada que ele vai sair
                    fire[i].x = jogador.pos_pers_tela+15 ;
                    fire[i].y = jogador.y+9;
                    fire[i].direcaotiro=jogador.direcao;
                    break;
                }
            }
        }

        if(key[KEY_S]){
             masked_blit(jogador.person,buffer,0+(jogador.i*jogador.ix),120+(jogador.i*jogador.iy),jogador.pos_pers_tela,(int)jogador.y,31,32);
        }//FIM KEY_NULL

        //
        clear_to_color(buffer,makecol(255,255,255));

//imprime as sprites

        int nsprite=tela.sprite_atual;//pega a sprite atual

        for(int i=0;i<3;i++){
            //começa imprimindo a sprite atual usando um buffer circular
            blit  (tela.sprites[nsprite],buffer,0,0,(i*800)+tela.pos_sprite,0,800,400);
            nsprite=(nsprite+1) % 3;//atualiza o numero da sprite
        }

        //imprime o jogador
        if(jogador.direcao==1){
            if(jogador.ativo){
                if (AtualizaTempoP(300)){
                        jogador.i=(jogador.i+1)%2;
                }
                masked_blit(jogador.person,buffer,320+(jogador.i*jogador.ix),0+(jogador.i*jogador.iy),jogador.pos_pers_tela,(int)jogador.y,31,32);
            }
        }
        if(jogador.direcao==-1){
            if(jogador.ativo){
                if (AtualizaTempoP(300)){
                    jogador.i=(jogador.i+1)%2;
                }
                masked_blit(jogador.person,buffer,320+(jogador.i*jogador.ix),64+(jogador.i*jogador.iy),jogador.pos_pers_tela,(int)jogador.y,31,32);
            }
        }
        if(jogador.direcao==0){
            masked_blit(jogador.person,buffer,320+(jogador.i*jogador.ix),0+(jogador.i*jogador.iy),jogador.pos_pers_tela,(int)jogador.y,31,32);
        }

    if(jogador.direcao==1){
        if (jogador.direcao!=jogador.direcaold){

            jogador.ix=30;
            jogador.iy=0;
            jogador.i=0;
            jogador.direcaold=jogador.direcao;
        }
    }

    if(jogador.direcao==-1){
        if (jogador.direcao!=jogador.direcaold){

            jogador.ix=30;
            jogador.iy=0;
            jogador.i=0;
            jogador.direcaold=jogador.direcao;
        }
    }

    if(jogador.direcao==0){
        if (jogador.direcao!=jogador.direcaold){
            jogador.ix=30;
            jogador.iy=0;
            jogador.i=0;
            jogador.direcaold=jogador.direcao;
        }
    }

            for (int i = 0; i < 10; i++){

                if (fire[i].ativo == ATIVO && fire[i].direcaotiro==1){
                    fire[i].x ++ ;
                    if(fire[i].x<0 || fire[i].x>800)fire[i].ativo=NAO_ATIVO;
                    masked_blit(fire[0].tiro, buffer,0,0,fire[i].x,fire[i].y,15,15);
                }
                 if (fire[i].ativo == ATIVO && fire[i].direcaotiro==-1){
                    fire[i].x -- ;
                    if(fire[i].x<0 || fire[i].x>800)fire[i].ativo=NAO_ATIVO;
                    masked_blit(fire[0].tiro, buffer,0,0,fire[i].x,fire[i].y,15,15);
                }
            }
            //imprime inimigo
                inimigo.ix=30;
                inimigo.iy=0;
                inimigo.i=0;
                inimigo.direcaold=inimigo.direcao;
                int x;
                if(jogador.pos_pers_mundo++){
                    x--;
    masked_blit(inimigo.inim,buffer,0+(inimigo.i*inimigo.x),0+(inimigo.i*inimigo.x),inimigo.pos_pers_mundo+30,(int)inimigo.y,31,32);

                }

        //imprime tudo no monitor


  blit(buffer, screen,0,0,0,0,800,400);


     }

};







void init() {
    int depth, res;
    allegro_init();
    depth = desktop_color_depth();
    if (depth == 0) depth = 32;
    set_color_depth(depth);
    res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 400, 0, 0);
    if (res != 0) {
        allegro_message(allegro_error);
        exit(-1);
}

install_timer();
install_keyboard();
install_mouse();
/* add other initializations here */
}

void deinit() {
    clear_keybuf();
/* add other deinitializations here */
}
void iniciar_tiros(){
    int i;
    for (i = 0; i < 10; i++){
        fire[i].ativo = NAO_ATIVO;
        fire[i].vel = 40;
    }
}
void show_tiros(tiro*fire, BITMAP *buffer){
    int i;
    for (i = 0; i < 10; i++){
        if (fire[0].ativo == ATIVO){
            draw_sprite(buffer, fire[0].tiro, fire[i].x, fire[i].y);

            if (fire[i].x < buffer->w){
                fire[i].x += fire[i].vel;
            }
            else{
                fire[i].ativo = NAO_ATIVO;
            }
        }
    }
}

void atirar(){
    int i;
    for (i = 0; i < 10; i++)
    {
        if (fire[i].ativo == NAO_ATIVO)
        {
            fire[i].ativo = ATIVO;

            fire[i].x = jogador.pos_pers_tela;
            fire[i].y = jogador.y;

            break;
        }
    }
}

