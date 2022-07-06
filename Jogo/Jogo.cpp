#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "cmath"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

using namespace std;

vector <string> directionalCommands{ "ESQUERDA", "DIREITA", "ATRAS" , "FRENTE"};
vector <string> movementLowCommands{ "AGACHAR", "DESLIZAR", "ABAIXAR" };
vector <string> movementHighCommands{ "ESCALAR", "PULAR","SALTAR" ,"SUBIR"};
vector <string> menuCommands{ "COMECAR", "SAIR", "RETORNAR","COMANDOS"};
vector <string> interactCommands{ "INTERAGIR" };

class command
{
    ALLEGRO_TIMER* actionTimer = NULL;
    ALLEGRO_EVENT_QUEUE* q2 = NULL;
public:
    command()
    {
        q2 = al_create_event_queue();
        actionTimer = al_create_timer(1);
        al_register_event_source(q2, al_get_timer_event_source(actionTimer));
    }
    ~command()
    {
        al_destroy_event_queue(q2);
    }
    int getCurrentTime()
    {
        int64_t actionTimeCount = 10 - al_get_timer_count(actionTimer);
        return actionTimeCount;
    }
    bool getActionTimerStarted()
    {
        return al_get_timer_started(actionTimer);
    }
    void stopActionTimer()
    {
        al_destroy_timer(actionTimer);
    }
    virtual int changeScene(int scene) = 0;
protected:
    void actionTimeManager(int currentScene)
    {
        switch (currentScene)
        {
        case 7:
        case 9:
            if (al_get_timer_started(actionTimer))
            {
                stopActionTimer();
                actionTimer = al_create_timer(1);
                al_register_event_source(q2, al_get_timer_event_source(actionTimer));
            }
            al_start_timer(actionTimer);
            break;
        case 11:
            stopActionTimer();
            break;
        }
    }
};

class movementLowCommand: virtual public command
{
    int stressPernas=0;
public:
    movementLowCommand():command()
    {}
    ~movementLowCommand()
    {}
    bool lowScenes(int currentScene, string word, int &progressScene)
    {
        bool confirmation = false;
        switch (currentScene)
        {
        case 1:
            progressScene = 3;
            stressPernas++;
            confirmation = true;
            break;
        case 7:
            progressScene = 9;
            stressPernas++;
            actionTimeManager(progressScene);
            confirmation = true;
            break;
        default:
            break;
        }
        return confirmation;
    }
    int changeScene(int scene)
    {
        return scene;
    }
};

class movementHighCommand : virtual public command
{
    int stressBracos=0;
public:
    movementHighCommand():command()
    {}
    virtual ~movementHighCommand()
    {}
    bool highScenes(int currentScene, string word, int &progressScene)
    {
        bool confirmation = false;
        switch (currentScene)
        {
        case 5:
            progressScene = 6;
            stressBracos++;
            confirmation = true;
            break;
        case 9:
            if (word == "SALTAR" || word == "PULAR")
            {
                progressScene = 11;
                actionTimeManager(progressScene);
                confirmation = true;
            }
            break;
        default:
            break;
        }
        return confirmation;
    }
    int changeScene(int scene)
    {
        return scene;
    }
};

class directionalCommand : virtual public command
{
public:
    directionalCommand():command()
    {}
    virtual ~directionalCommand()
    {}
    bool directionalScenes(int currentScene,string word, int &progressScene)
    {
        bool confirmation = false;
        switch (currentScene)
        {
        case 1:
            if (word == "DIREITA")
            {
                progressScene = 2;
                confirmation = true;
            }
            break;
        case 2:
        case 4:
            if (word == "ATRAS")
            {
                progressScene = 1;
                confirmation = true;
            }
            break;
        case 3:
            if (word == "FRENTE")
            {
                progressScene = 5;
                confirmation = true;
            }
            break;
        case 6:
            if (word == "FRENTE")
            {
                progressScene = 7;
                actionTimeManager(progressScene);
                confirmation = true;
            }
            break;
        case 11:
            if (word == "ESQUERDA")
            {
                progressScene = 13;
                confirmation = true;
            }
            else
                if (word == "DIREITA")
                {
                    progressScene = 12;
                    confirmation = true;
                }
            break;
        case 13:
            if (word == "DIREITA")
            {
                progressScene = 12;
                confirmation = true;
            }
            break;
        default:
            break;
        }
        return confirmation;
    }
    int changeScene(int scene)
    {
        return scene;
    }
};

class menuCommand : virtual public command
{
public:
    menuCommand():command()
    {}
    virtual ~menuCommand()
    {
    }
    bool menuScenes(int currentScene, string word, int &progressScene)
    {
        bool confirmation = false;
        switch (currentScene)
        {
        case 0:
            if (word == "COMECAR")
            {
                progressScene = 1;
                confirmation = true;
            }
            break;
        case 50:
            if (word == "RETORNAR")
            {
                confirmation = true;
            }
        default:
            if (word == "COMANDOS" && currentScene != 7 && currentScene != 9)
            {
                progressScene = 50;
                confirmation = true;
            }
        }
        return confirmation;
    }
    int changeScene(int scene)
    {
        return scene;
    }
};

class interactCommand : virtual public command
{
public:
    interactCommand():command()
    {}
    ~interactCommand()
    {}
    bool interactScenes(int currentScene, string word, int &progressScene)
    {
        bool confirmation = false;
        switch (currentScene)
        {
        case 2:
            progressScene = 4;
            confirmation = true;
            break;
        case 12:
            progressScene = 14;
            confirmation = true;
        default:
            break;
        }
        return confirmation;
    }
    int changeScene(int scene)
    {
        return scene;
    }
};

class boneco : public movementHighCommand, public movementLowCommand, public menuCommand, public directionalCommand, public interactCommand
{
public:
    boneco():command()
    {};
    
    int changeScene(int scene)
    {
        return scene;
    }
    int specialScenes(int scene)
    {
        switch (scene)
        {
        case 8:
        case 10:
        case 14:
            return 1;
            break;
        case 50:
            return 2;
        default:
            return 0;
            break;
        }
    }
};

int findCommand(string currentCommand)
{
    vector<string>::iterator it;
    string ser = currentCommand;
    it = find(movementLowCommands.begin(), movementLowCommands.end(), ser);
    if (it != movementLowCommands.end())
    {
        return (2);
    }
    else
    {
        it = find(movementHighCommands.begin(), movementHighCommands.end(), ser);
        if (it != movementHighCommands.end())
        {
            return(3);
        }
        else
        {
            it = find(directionalCommands.begin(), directionalCommands.end(), ser);
            if (it != directionalCommands.end())
            {
                return(1);
            }
            else
            {
                it = find(menuCommands.begin(), menuCommands.end(), ser);
                if (it != menuCommands.end()) {
                    return 0;
                }
                else
                {
                    it = find(interactCommands.begin(), interactCommands.end(), ser);
                        if (it != interactCommands.end())
                        {
                            return 4;
                        }
                        else
                        {
                            return -1;
                        }
                }
            }
        }
    }
}

void sceneDraw(int currentScene,ALLEGRO_FONT *font)
{
    char text[1000] = "";
    switch (currentScene)
    {
    case 0:
        al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, NULL, "Bem vindo ao programa de testes #17!");
        al_draw_multiline_text(font, al_map_rgb(255, 255, 255), 0, 20,1920,20, NULL,"Neste programa voce controla - ra um boneco ficticio com o objetivo de guia - lo ate o fim. Para guiar o boneco voce recebera uma mensagem dele descrevendo o que o boneco esta vendo naquele milisegundo. A partir da descricao, voce devera dar um comando relevante para que ele aja de acordo com a situacao. Para isso voce escrevera ou uma direcao cardinal(esquerda, direita, etc) ou um verbo no infinitivo relacionado a acao que voce quer que ele faca. Tenha em mente que, apesar de ser um mero boneco, ele sera capaz de discernir se o comando recebido e relevante a situacao ou nao, como por exemplo receber o comando 'pular' em vez de 'interagir' ao encontrar uma porta por exemplo, portanto tome cuidado! Alem disso, repetir muitas vezes o mesmo tipo de comando por desgastar o boneco, entao varie suas ordens! Voce pode a qualquer momento(que nao seja em cenas de acao) escrever 'comandos' para ver os comandos compreendidos pelo boneco. Isto e tudo. Quando estiver pronto para prosseguir, escreva 'comecar'.");
        break;
    case 1:
        strcpy_s(text, "Uma sala quadrada sem nada de especial se encontra ao redor. Uma porta se encontra na direita da sala, enquanto que imediatamente na frente existe um buraco de cerca de 1 metro de altura e largura.");
        al_draw_multiline_text(font, al_map_rgb(255, 255, 255), 0, 0, 1920, 20, NULL, text);
        break;
    case 2:
        strcpy_s(text, "A porta esta na sua frente.");
        al_draw_multiline_text(font, al_map_rgb(255, 255, 255), 0, 0, 1920, 20, NULL, text);
        break;
    case 3:
        strcpy_s(text, "O boneco passa pelo buraco sem problema. Um longo corredor esta a frente.");
        al_draw_multiline_text(font, al_map_rgb(255, 255, 255), 0, 0, 1920, 20, NULL, text);
        break;
    case 4:
        strcpy_s(text, "O boneco tenta abrir a porta, mas ela parece estar trancada.");
        al_draw_multiline_text(font, al_map_rgb(255, 255, 255), 0, 0, 1920, 20, NULL, text);
        break;
    case 5:
        strcpy_s(text, "No fim do corredor o boneco encontra uma parede, com uma janela aberta no alto.");
        al_draw_multiline_text(font, al_map_rgb(255, 255, 255), 0, 0, 1920, 20, NULL, text);
        break;
    case 6:
        strcpy_s(text, "O boneco passa pela janela e desce em um outro corredor. Ele continua mais a frente.");
        al_draw_multiline_text(font, al_map_rgb(255, 255, 255), 0, 0, 1920, 20, NULL, text);
        break;
    case 7:
        strcpy_s(text, "Depois de andar alguns passos, o boneco ouve algo atras dele, e quando ele vira ele conseguer ver uma tabua com espinhos vindo em direcao ao seu rosto!");
        al_draw_multiline_text(font, al_map_rgb(255, 255, 255), 0, 0, 1920, 20, NULL, text);
        break;
    case 8:
        strcpy_s(text, "A tabua acerta o rosto do boneco, destruindo-o.");
        al_draw_multiline_text(font, al_map_rgb(255, 255, 255), 0, 0, 1920, 20, NULL, text);
        break;
    case 9:
        strcpy_s(text, "O boneco desvia da tabua e continua indo para a frente, quando uma outra tabua vem de frente em direcao a suas pernas!");
        al_draw_multiline_text(font, al_map_rgb(255, 255, 255), 0, 0, 1920, 20, NULL, text);
        break;
    case 10:
        strcpy_s(text, "A tabua acerta as pernas do boneco, as decepando.");
        al_draw_multiline_text(font, al_map_rgb(255, 255, 255), 0, 0, 1920, 20, NULL, text);
        break;
    case 11:
        strcpy_s(text, "O boneco salta sobre a tabua e chega numa bifurcacao indo para a esquerda e direita.");
        al_draw_multiline_text(font, al_map_rgb(255, 255, 255), 0, 0, 1920, 20, NULL, text);
        break;
    case 12:
        strcpy_s(text, "O boneco vai para a direita, onde ele acha uma porta.");
        al_draw_multiline_text(font, al_map_rgb(255, 255, 255), 0, 0, 1920, 20, NULL, text);
        break;
    case 13:
        strcpy_s(text, "O boneco vai para a esquerda, e e um caminho sem saida! So resta o caminho para a direita agora.");
        al_draw_multiline_text(font, al_map_rgb(255, 255, 255), 0, 0, 1920, 20, NULL, text);
        break;
    case 14:
        strcpy_s(text, "O boneco abre a porta e escapa do cenario!");
        al_draw_multiline_text(font, al_map_rgb(255, 255, 255), 0, 0, 1920, 20, NULL, text);
        break;
    case 50:
        strcpy_s(text, "ESQUERDA, DIREITA, ATRAS, FRENTE || AGACHAR, DESLIZAR, ABAIXAR || ESCALAR, SUBIR, PULAR, SALTAR || INTERAGIR");
        al_draw_multiline_text(font, al_map_rgb(255, 255, 255), 0, 0, 1920, 20, NULL, text);
    }

}

void systemDraw(ALLEGRO_FONT* font,boneco* boneck,int specialScene)
{
    if (boneck->getActionTimerStarted())
    {
        string s = to_string(boneck->getCurrentTime());
        const char* time = s.c_str();
        al_draw_text(font, al_map_rgb(255, 255, 255), 1800, 900, NULL, time);
    }

    if (specialScene != 0)
    {
        switch (specialScene)
        {
        case 1:
            al_draw_text(font, al_map_rgb(255, 255, 255), 960, 540, NULL, "Escreva 'Sair' ou clique no 'x' para fechar o jogo.");
            break;
        case 2:
            al_draw_text(font, al_map_rgb(255, 255, 255), 960, 540, NULL, "Escreva 'Retornar' para voltar a cena anterior.");
            break;
        }
    
    }
}
void drawTyping(ALLEGRO_FONT* font,string typing)
{
    const char* c = typing.c_str();
    al_draw_text(font, al_map_rgb(255, 255, 255), 0, 200, NULL,c);
    
}

#define KEY_SEEN     1
#define KEY_RELEASED 2

unsigned char key[ALLEGRO_KEY_MAX];
void keyboard_init()
{
    memset(key, 0, sizeof(key));
}

void keyboard_update(ALLEGRO_EVENT* event,string &typing )
{
    const char* c = typing.c_str();
    switch (event->type)
    {
    case ALLEGRO_EVENT_TIMER:
        for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
            key[i] &= KEY_SEEN;
        break;

    case ALLEGRO_EVENT_KEY_DOWN:
        key[event->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
        break;
    case ALLEGRO_EVENT_KEY_UP:
        key[event->keyboard.keycode] &= KEY_RELEASED;
        break;
    case ALLEGRO_EVENT_KEY_CHAR:

        if (event->keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
        {
            if (typing.length() != 0)
            {
                typing.pop_back();
            }
        }
        else
        {
            c = al_keycode_to_name(event->keyboard.keycode);
            typing = typing + c;
        }
    }
}

int main()
{
    int commandType, currentScene, progressScene, actionTimerCount, returnScene;
    currentScene = commandType = actionTimerCount = returnScene = 0;
    progressScene = -1;
    bool redraw, progressConfirmed, done;
    string typing;

    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();

    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_TIMER* timer;
    ALLEGRO_FONT* font;
    boneco* boneck;
    boneck = new boneco();
    command* progress=NULL;
    movementHighCommand* currentHighCommand = new movementHighCommand();
    movementLowCommand* currentLowCommand = new movementLowCommand();
    menuCommand* currentMenuCommand = new menuCommand();
    directionalCommand* currentDirectionalCommand = new directionalCommand();
    interactCommand* currentInteractCommand = new interactCommand();

        font = al_load_ttf_font("mechanical_font.ttf", 20, 0);
        if (font == NULL) {
            return 1;
        }
    
    queue = al_create_event_queue();

    display = al_create_display(1920, 1080);
    if (display == NULL) {
        return 0;
    }

    timer = al_create_timer(1.0/30.0);


    al_install_keyboard();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    keyboard_init();
    redraw = true;
    done = progressConfirmed = false;
    al_start_timer(timer);

    //Programa rodando
    while (1)
    {

        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        switch (event.type)
        {
           case ALLEGRO_EVENT_TIMER:
               redraw = true;
               break;
           case ALLEGRO_EVENT_DISPLAY_CLOSE:
               done = true;
               break;
        }

        if (typing == "SAIR")
            done = true;

        if (done)
            break;

        keyboard_update(&event,typing);
        if (redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            sceneDraw(currentScene,font);
            systemDraw(font,boneck,boneck->specialScenes(currentScene));
            drawTyping (font,typing);
            al_flip_display();
            redraw = false;
        }
        if (boneck->getActionTimerStarted() && boneck->getCurrentTime() == 0)
        {
            switch (currentScene)
            {
            case 7:
                currentScene = 8;
                typing = "";
                boneck->stopActionTimer();
                break;
            case 9:
                currentScene = 10;
                typing = "";
                boneck->stopActionTimer();
                break;
            }
        }
        if (findCommand(typing) != -1)
        {
            switch (findCommand(typing))
            {
            case 0:
                progressConfirmed = boneck->menuScenes(currentScene, typing, progressScene);
                progress = currentMenuCommand;
                break;
            case 1:
                progressConfirmed = boneck->directionalScenes(currentScene, typing, progressScene);
                progress = currentDirectionalCommand;
                break;
            case 2:
                progressConfirmed = boneck->lowScenes(currentScene, typing, progressScene);
                progress = currentLowCommand;
                break;
            case 3:
                progressConfirmed = boneck->highScenes(currentScene, typing, progressScene);
                progress = currentHighCommand;
                break;
            case 4:
                progressConfirmed = boneck->interactScenes(currentScene, typing, progressScene);
                progress = currentInteractCommand;
                break;
            }
            if (progressConfirmed)
            {
                if (typing == "COMANDOS")
                {
                    returnScene = currentScene;
                }
                if (typing == "RETORNAR" && currentScene == 50)
                {
                    currentScene = progress->changeScene(returnScene);
                    typing = "";
                    progressConfirmed = false;
                }
                else
                {
                    currentScene = progress->changeScene(progressScene);
                    typing = "";
                    progressConfirmed = false;
                }
            }
        }
    }
    
    al_destroy_display(display);
    al_uninstall_keyboard();
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    delete boneck;
    delete currentDirectionalCommand;
    delete currentHighCommand;
    delete currentInteractCommand;
    delete currentLowCommand;
    delete currentMenuCommand;

    return 0;
}