#include <bits/stdc++.h>
using namespace std;

class objects{
    public :
        objects(){
            obj[-1].name="+";
            obj[0].name="Path";
            obj[1].name="Wall";
            obj[3].name="Chest";
            obj[4].name="Chest";
            obj[0].sym="\x1B[31m.\033[0m"; //land
            obj[1].sym="\x1B[92m#\033[0m"; //wall
            obj[3].sym="\x1B[33mC\033[0m"; //Chest
            obj[4].sym="X";                //Mimic
            obj[4].type="enemy";
            obj[3].type="object";
            obj[9]={9,"Kwek A Duck","npc","D"};
            obj[9].dialoge.push_back("Kwak ? Kwak ?");
            obj[9].dialoge.push_back(".........");
            obj[9].dialoge.push_back("Sorry I thought you were A duck too...");
            obj[9].dialoge.push_back("?Soo silent in here right??");
            obj[9].dialoge.push_back("+Thank you for agree with that, so your name its "+obj[7].name+"?");
            obj[9].dialoge.push_back("+Nice to meet you "+obj[7].name);
            obj[9].dialoge.push_back("-How?? So silent here....");
            obj[9].dialoge.push_back("-If you think soo, okay then");
            obj[9].dialoge.push_back("Kwek Kwek Kwek Kwek Kwek");
            obj[6].sym="F";
            obj[6].name="Finish Portal";
            obj[6].type="portal";

        }
        struct desc{
            int id; //same with number assign at map
            string name; // Name this object
            string type; // static, object, player, enemy, or npc
            string sym; // at screen
            double hp,level,hunger,stamina;
            vector<string> dialoge;
        };
        
        map<int,desc> obj;
};
class Items{
    public :
        struct weapon{
            string name;
            double damage,stamina;
        };
        vector<weapon> get_weapon(){
            return list_weapon;
        }
        void add_weapon(weapon new_weapon){
            list_weapon.push_back(new_weapon);
        }
    private :
        vector<weapon> list_weapon;
};
class cave : public objects{
    public :
        cave(int n,int m){
            this->n=max(10,n);
            this->m=max(15,m);
            generate();
        }
    protected :
        int n=20;
        int m=50;
        int maps_filter=1; //filter
        vector<vector<int>> maps;
        vector<vector<bool>> vis;
        vector<vector<int>> filter;//Experimental
        void dfs(int i,int j){

            if (i>=n||j>=m||i<0||j<0||vis[i][j]==1||maps[i][j]==1){
                return;
            }
            vis[i][j]=1;
            dfs(i+1,j);
            dfs(i-1,j);
            dfs(i,j+1);
            dfs(i,j-1);
        }
        void generate(){
            srand(time(0));
            maps.assign(n,vector<int>(m));
            filter.assign(n,vector<int>(m,maps_filter));
            for (int i=0;i<n;i++){
                for (int j=0;j<m;j++){
                    maps[i][j]=rand()%2;
                }
            }
            int smoth=3;
            int dx[8]={1,-1,0,0,1,-1,-1,1};
            int dy[8]={0,0,1,-1,1,-1,1,-1};
            while (smoth--){
                for (int i=0;i<n;i++){
                    for (int j=0;j<m;j++){
                        int paths=0;
                        int walls=0;
                        if (!maps[i][j])paths+=1;
                        else walls+=1;
                        for (int k=0;k<8;k++){
                            int nx=i+dx[k];
                            int ny=j+dy[k];
                            if (nx<0||nx>=n||ny<0||ny>=m)continue;
                            else {
                                if (!maps[nx][ny])paths+=1;
                                else walls+=1;
                            }
                        }
                        if (paths>walls)maps[i][j]=0;
                        else maps[i][j]=1;
                    }
                }
            }
            struct cnt{
                int x,y;
            };
            stack<cnt> connect;
            connect.push({0,0});
            vis.assign(n,vector<bool>(m,0));
            for (int i=0;i<n;i++){
                for (int j=0;j<m;j++){
                    if (!(vis[i][j]+maps[i][j])){
                        connect.push({i,j});
                        dfs(i,j);
                    }
                }
            }
            // connect.push({0,0});
            connect.push({n-1,m-1});
            int sx=0,sy=0;
            while (!connect.empty()){
                stack<int> correct_path;
                auto [x,y]=connect.top();
                connect.pop();
                int px=sx,py=sy;
                while (sx!=x||sy!=y){
                    int gnrt=rand()%2;
                    int hx=max(-1,min(x-sx,1));
                    int hy=max(-1,min(y-sy,1));
                    if (sx==x){
                        correct_path.push(hy*1);
                        sy+=hy;
                        continue;
                    }
                    else if (sy==y){
                        correct_path.push(hx*2);
                        sx+=hx;
                        continue;
                    }
                    if (!gnrt){
                        correct_path.push(hx*2);
                        sx+=hx;
                    } else {
                        correct_path.push(hy*1);
                        sy+=hy;
                    }
                }
                maps[px][py]=0;
                while (!correct_path.empty()){
                    int move=correct_path.top();
                    correct_path.pop();
                    // cout<<move<<" ";
                    if (move==-2||move==2)px+=move/2;
                    else py+=move;
                    // if (x<0||y<0||x>=n||y>=m)break;
                    maps[px][py]=0;
                    // if (x+1<n)maps[x+1][y]='.';
                }
                sx=x;
                sy=y;
            }
            place_object(3,max(3,(m+n)/3));
            place_object(4,4);
            place_object(9,10);
            maps[n-1][m-1]=6;
        }

        void place_object(int id,int amount){
            int brk=0;
            while (true){
                brk+=1;
                int x=rand()%n;
                int y=rand()%m;
                if (maps[x][y]==0){
                    maps[x][y]=id;
                    amount-=1;
                }
                if (brk>min(1000*amount,25000)||amount==0)return;
            }
        }

};

class Player : public objects, public Items{
    public :
        desc description;
        void set_desc(desc o){
            this->description=o;
        }
        desc get_desc(){
            return description;
        }
        int px=0,py=0;
};
class Enemy : public objects {
    public :
        void set_desc(desc o){
            this->description=o;
        }
        desc get_desc(){
            return description;
        }
        desc description;
};

class Screen : protected cave, public Items{
    protected :
        Screen(): cave(25,50){};
        void map_scene(vector<string> obj_nearby){
            while (true){
                system("clear");
                for (int i=0;i<n;i++){
                    if (i==0){
                        for (int x=0;x<m+2;x++){
                            cout<<"= ";
                            //if (x==m+1)cout<<"Object Nearby";
                        }
                        cout<<endl;
                    }
                for (int j=0;j<m;j++){
                    if (j==0)cout<<"= ";
                    if (filter[i][j]==1&&maps_filter==1)cout<<"X ";
                    else cout<<obj[maps[i][j]].sym<<" ";
                    if (j==m-1){
                        cout<<"=      ";
                        if (i==1)cout<<"==================================";
                        else if (i==2)cout<<"     Information Aoround You";
                        else if (i==3)cout<<"==================================";
                        else if (i>3&&i<8){
                            string pos_desc[4]={"Left  : ","Right : ","Up    : ","Down  : "};
                            cout<<pos_desc[i-4]<<obj_nearby[i-4];
                        } else if (i==9){
                            cout<<"\x1B[31mtype i to Interract with object\033[0m";
                        }
                    }
                } cout<<endl;
                if (i==n-1){
                    for (int x=0;x<m+2;x++)cout<<"= ";
                    cout<<endl;
                }
            }
            //generate();
            cout<<endl;
            break;
            }
        }
        void start_scene(){
            system("clear");
            ifstream file("assets/start_scene.txt");
            string s;
            while (getline(file,s)){
                for (char c:s)cout<<c<<" ";
                cout<<endl;
            }
            file.close();
        }
        void gameover_scene(){
            system("clear");
            ifstream file("assets/g_over.txt");
            string s;
            while (getline(file,s)){
                for (char c:s)cout<<c<<" ";
                cout<<endl;
            }
            cout<<"THe WARrior Try His BESt."<<endl;
            file.close();
        }

        bool battle_scene(Player pl,Enemy pe,string log){
            system("clear");
            ifstream file("assets/b_scene.txt");
            string s;
            desc p=pl.get_desc();
            desc e=pe.get_desc();
            vector<string> info_text={
                "============================",
                "Your Status, "+p.name,
                "Hp       : "+to_string(p.hp),
                "Level    : "+to_string(p.level),
                "Stamina  : "+to_string(p.stamina),
                "Hunger   : "+to_string(p.hunger),
                "============================",
                "Enemy Status, "+e.name,
                "Hp       : "+to_string(e.hp),
                "Level    : "+to_string(e.level),
                "Stamina  : "+to_string(e.stamina),
                "Hunger   : "+to_string(e.hunger)

            };
            int cnt=0;
            while (getline(file,s)){
                for (char c:s)cout<<c<<" ";
                if (cnt<info_text.size())cout<<info_text[cnt];
                cnt+=1;
                cout<<endl;
            }
            cout<<"\nType 1 to Attack (Stamina -10)\nType 2 to use Punch (Stamina -50)\n";
            // vector<weapon> list=pl.get_weapon();
            // int idx=1;
            // for (weapon wp : list){
            //     cout<<idx<<" "<<wp.name<<endl;
            //     idx+=1;
            // }
            cout<<log<<endl;
            file.close();
            if (p.hp<=0)return false;
            return true;
        }
        void finish_scene(){
            system("clear");
            ifstream file("assets/finish.txt");
            string s;
            while (getline(file,s)){
                for (char c:s)cout<<c<<" ";
                cout<<endl;
            }
            file.close();
        }
        void dialoge_scene(desc npc,string dialog){
            system("clear");
            ifstream file("assets/duck.txt");
            string s;
            while (getline(file,s)){
                for (char c:s)cout<<c<<" ";
                cout<<endl;
            }
            cout<<">> "+npc.name<<endl;
            cout<<"==================================================================================================="<<endl;
            cout<<dialog<<endl;
            cout<<"==================================================================================================="<<endl;
            file.close();
        }
};
class Game : protected Screen{
    stack<string> log;
    public :
        Game(){
            string name;
            int player_id=7;
            int enemy_id=-1;
            int object_id;
            int npc_id;
            int cnt_dialog=0;
            int tx=-1,ty=-1;
            char dialoge_neutral='n';
            Enemy enemy;
            Player player;
            player.add_weapon({"Mighty Sword",100,0});
            string player_sym;
            int scene=-1;
            string command;
            update_position(player.px,player.py,player_id);
            while (true){
                if (scene==-1){
                    while (true){
                        system("clear");
                        start_scene();
                        cout<<"Welcome To Dungeon Of RO"<<endl;
                        cout<<"Enter Your Name Please : (max 10 Character)"<<endl;
                        getline(cin>>ws,name);
                        if (name.size()<=10){
                            log.push("You wake up inside a mysterious cave. The only thing you remember was your Name, "+name+".");
                            break;
                        };
                        
                    }
                    string f_let="";
                    f_let+=name[0];
                    player_sym="\033[1;34m"+f_let+"\033[0m";

                    player.set_desc({player_id,name,"player",player_sym,100,1,100,100});
                    obj[player.description.id]=player.get_desc();
                    scene=0;
                } else if (scene==0){
                    update_position(player.px,player.py,player_id);
                    struct pos_f{
                        int x,y;
                    };
                    int x=player.px;
                    int y=player.py;
                    vector<pos_f> f={{x,y},{x+1,y},{x-1,y},{x,y+1},{x,y-1},{x+1,y+1},{x+1,y-1},{x-1,y+1},{x-1,y-1},{x+2,y},{x-2,y},{x,y+2},{x,y-2}};
                    for (auto [nx,ny]:f){
                        if (valid_move_filter(nx,ny,1))filter[nx][ny]=0;
                    }
                    vector<string> obj_nearby_player=check_object_nearby("get_obj",player.px,player.py);
                    map_scene(obj_nearby_player);
                    cout<<name<<endl;
                    cout<<"[LOG] : "<<log.top()<<endl;
                    cout<<"Type u to move up, d to move down, l to move left and r to move right"<<endl;
                    cout<<"Command : ";
                    cin>>command;
                    if (command=="i"){
                        int dx[4]={0,0,-1,1};
                        int dy[4]={-1,1,0,0};
                        int cnt=0;
                        vector<string> get_obj_id=check_object_nearby("get_id",player.px,player.py);
                        for (string s:get_obj_id){
                            tx=player.px+dx[cnt];
                            ty=player.py+dy[cnt];
                            cnt+=1;
                            if (s=="-1")continue;
                            int obj_id=stoi(s);
                            string obj_type=obj[obj_id].type;
                            if (obj_type=="enemy"){
                                enemy_id=obj_id;
                                enemy.set_desc({obj_id,obj[obj_id].name,obj[obj_id].type,obj[obj_id].sym,100,2,10,50});
                                scene=1;
                                log.push("You try to fight with "+obj[obj_id].name);
                                break;
                            } else if (obj_type=="object"){
                                scene=3;
                                object_id=obj_id;
                                log.push("You interact with object "+obj[obj_id].name);
                                break;
                            } else if (obj_type=="npc"){
                                scene=4;
                                npc_id=obj_id;
                                log.push("You Talked with stranger...");
                                dialoge_neutral='n';
                                cnt_dialog=0;
                                break;
                            } else if (obj_type=="portal"){
                                scene=5; 
                            } else {
                                log.push("You cant interact with this object");
                            }
                        }
                    } else if (command=="u"||command=="U"||command=="d"||command=="D"||command=="l"||command=="L"||command=="r"||command=="R"){
                        if (walk(command,player.px,player.py));
                        else log.push("You have tried to take that path and failed...");
                    } else if (command=="exit"){
                        ofstream file("log/log.txt");
                        while (!log.empty()){
                            file<<log.top()<<'\n';
                            log.pop();
                        }
                        file.close();
                        return;
                    } else if (command=="[filter_off]"){
                        maps_filter=0;
                    } else if (command=="[filter_on]"){
                        maps_filter=1;
                    }

                } else if (scene==1){
                    if (battle_scene(player,enemy,log.top()));
                    else gameover_scene();
                    cout<<name<<endl;
                    cout<<"Your move  : ";
                    cin>>command;
                    if (command=="1" && player.description.stamina-10>=0){
                        log.push("You Attack "+enemy.description.name+", your stamina -10, damage 20\n and enemy attack you, hp -10");
                        enemy.description.hp-=20;
                        player.description.hp-=10;
                        player.description.stamina-=10;
                    } else if (command=="2" && player.description.stamina-50>=0){
                        if (rand()%2){
                            enemy.description.hp-=70;
                            player.description.hp-=20;
                            log.push("You Attack "+enemy.description.name+", your stamina -50, damage 70\n and enemy attack you, hp -20");
                        } else {
                            log.push("You Attack "+enemy.description.name+", but the enemy use defense, Your stamina -50, damage 0");
                        }
                        player.description.stamina-=50;
                    }
                    if (player.description.hp<=0||player.description.stamina<=0){
                        gameover_scene();
                        log.push("You died without knowing anything...");
                        ofstream file("log/log.txt");
                        while (!log.empty()){
                            file<<log.top()<<'\n';
                            log.pop();
                        }
                        file.close();
                        return;
                        scene=0;
                    }
                    if (enemy.description.hp<=0){
                        maps[tx][ty]=0;
                        log.push("You have won against "+enemy.description.name);
                        scene=0;
                    }
                } else if (scene==3){
                    log.push("You opened a chest and got Potion, adn then use it, your hp now full");
                    player.description.hp=100;
                    maps[tx][ty]=0;
                    scene=0;
                } else if (scene==4){
                    maps[tx][ty]=0;
                    if (cnt_dialog==obj[npc_id].dialoge.size()){
                        scene=0;
                        log.push("The conversation has ended.");
                        continue;
                    }
                    string conver=obj[npc_id].dialoge[cnt_dialog];
                    if (conver[0]==dialoge_neutral){
                        cnt_dialog+=1;
                        continue;
                    }
                    dialoge_scene(obj[npc_id],conver);
                    if (conver[0]!='?'){
                        cout<<"Type x to next "<<endl;
                        cin>>command;
                        log.push(obj[npc_id].name+" said "+conver);
                        if (command=="x"){
                            cnt_dialog+=1;
                        }
                    } else {
                        cout<<"Type + for yes and - for no "<<endl;
                        cin>>command;
                        log.push(obj[npc_id].name+" ask "+conver);
                        if (command=="+"){
                            dialoge_neutral='-';
                            cnt_dialog+=1;
                        } else if (command=="-"){
                            dialoge_neutral='+';
                            cnt_dialog+=1;
                        }
                    }
                } else if (scene==5){
                    log.push("You end your journey...");
                    ofstream file("log/log.txt");
                        while (!log.empty()){
                            file<<log.top()<<'\n';
                            log.pop();
                        }
                        file.close();
                    finish_scene();
                    return;
                }
            }
        }
        vector<string> check_object_nearby(string task,int x,int y){
            int dx[4]={0,0,-1,1};
            int dy[4]={-1,1,0,0};
            vector<string> obj_nearby={"","","",""};
            vector<string> obj_nearby_id={"-1","-1","-1","-1"};
            for (int i=0;i<4;i++){
                int nx=x+dx[i];
                int ny=y+dy[i];
                if (nx<0||nx>=n||ny<0||ny>=m){
                    obj_nearby[i]="something you cant see...";
                    continue;
                }
                obj_nearby[i]=obj[maps[nx][ny]].name;
                obj_nearby_id[i]=to_string(maps[nx][ny]);
            }
            if (task=="get_obj")return obj_nearby;
            else if (task=="get_id")return obj_nearby_id;
            return {"","","",""};
        }
        void update_position(int &x, int &y,int id){
            maps[x][y]=obj[id].id;
        }
        bool valid_move(int px, int py,int id){
            if (px<0||px>=n||py<0||py>=m||maps[px][py]!=id)return false;
            else return true;
        }
        bool valid_move_filter(int px, int py,int id){
            if (px<0||px>=n||py<0||py>=m||filter[px][py]!=id)return false;
            else return true;
        }
        bool walk(string direct,int &x,int &y){
            update_position(x,y,0);

            if ((direct=="D"||direct=="d")&&valid_move(x+1,y,0)){
                x+=1;
                log.push("You have walked to the Down path.");
            } else if ((direct=="U"||direct=="u")&&valid_move(x-1,y,0)){
                x-=1;
                log.push("You have walked to the Up path.");
            } else if ((direct=="L"||direct=="l")&&valid_move(x,y-1,0)){
                y-=1;
                log.push("You have walked to the Left path.");
            } else if ((direct=="R"||direct=="r")&&valid_move(x,y+1,0)){
                y+=1;
                log.push("You have walked to the Right path.");
            } else {
                return false;
            }
            return true;
        }
};
int main(){
    cout<<"WELCOME TO THE DUNGEON OF RO >_<"<<endl;
    Game game_of_ro;
    // The prince was wake up in the forbidden world of nothing
}
