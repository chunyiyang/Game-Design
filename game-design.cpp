#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;


class BaseWeapon;
class BaseWarrior;
class city;

int currenttime;
int num_city;
int arrow_power, lion_minus;
vector<city> cities;
vector<int>  lifepoints;
vector<int>  strengths;
vector<BaseWeapon *> pWeapons;
void PrintTime();
void PrintSubject(BaseWarrior * pWarrior);
void PrintLifePoint(BaseWarrior * pWarrior);
bool MoveForward();
void DragonYell();
void AfterWar();
void ReportWeapon();

#define MAX_WARRIORS 5
#define MAX_TOOL 3
#define RED_WIN 1
#define BLUE_WIN 2
#define TIE 0
const char WARRIOR_NAMES[MAX_WARRIORS][256] = {
 "dragon",
 "ninja",
 "iceman",
 "lion",
 "wolf"
};

const char WARRIOR_TOOL[MAX_TOOL][256] = {
    "sword",
    "bomb",
    "arrow"
};
class BaseWeapon{
    protected:
        int count;
        int power;
        int type;
        string name;  
    public: 
        BaseWeapon(int type_, int widx_){
            type = type_;
            count = 3;
            switch (type) {
                case 0: name = "sword";
                    power = strengths[widx_]/5;
                    break;
                case 1: name = "bomb";
                    count = 1;
                    break;
                case 2: name = "arrow";
                    power = arrow_power;
                    cout << "arrow_power = " << arrow_power << endl;
                    break;
            }
        }
        int GetType() {return type;}
        int GetCount(){return count;}
        int GetPower(){return power;}
        string GetName(){return name;}
        bool UseUp(){
            bool bUseUp = false;
            switch (type) {
                case 0: if(power<=0){ bUseUp = true; }
                    break;
                case 1: if(count<=0){ bUseUp = true; }
                    break;
                case 2: if(count<=0){ bUseUp = true; }
                    break;
            }
            return bUseUp;
        }
        void UseWeapon(){
            switch (type) {
                case 0: power = power*0.8;
                    break;
                case 1: count--;
                    break;
                case 2: count--;
                    break;
            }            
        }
};

class city {
    protected:
        int Point;
        int PreResult;   // 0 = tie or no war, 1 = red team win, 2 = blue team win
        int CurResult;   // 0 = tie or no war, 1 = red team win, 2 = blue team win
        bool Renew;
    public:
        BaseWarrior * pRedWarrior;
        BaseWarrior * pBlueWarrior;
        BaseWarrior * pExtraWarrior;
        int flag ;   // 1 = red team, 2 = blue team, 0 = not decided;
        int city_id;
        city(int city_id_){
            city_id = city_id_;
            flag = 0;
            pRedWarrior = NULL;
            pBlueWarrior = NULL;
            PreResult = 0;
            Renew = false;
        }
        void CreatePoint(){
            Point += 10;
        }
        void RedEnter(BaseWarrior *pWarrior);
        void BlueEnter(BaseWarrior *pWarrior);
        void RedLeave(BaseWarrior *pWarrior);
        void BlueLeave(BaseWarrior *pWarrior);
        int PointTaken();
        int RedPointTaken();
        int BluePointTaken();
        void CityWarResult(int warresult);
        void ChangeFlags();
};

int city::PointTaken(){
    int result = Point;
    Point = 0;
    return result;
}
void city::CityWarResult(int warresult){
    CurResult = warresult;
    Renew = true;
}
void city::ChangeFlags(){
        if (city_id == 5){
            cout << "city 5 PreResult = " << PreResult << " curresult = " << CurResult << endl;
        }
        if (Renew && (CurResult == PreResult) && (CurResult != 0) && (CurResult != flag)){
            flag = CurResult;
            printf("%03d:%02d ", currenttime/60, currenttime%60);
            if(flag==RED_WIN){
                cout << "red ";
            }
            else{
                cout << "blue ";
            }
            cout << "flag raised in city " <<city_id<< endl;
        }
        if (Renew){ 
            PreResult = CurResult; 
            CurResult = 0;
            Renew = false; 
            if (city_id == 5){
                cout << "city 5 PreResult = " << PreResult << " curresult = " << CurResult << endl;
            }
        }
        if (city_id == 5){
            cout << "city 5 PreResult = " << PreResult << " curresult = " << CurResult << endl;
        }
}
void city::RedEnter(BaseWarrior *pWarrior){
    pRedWarrior = pWarrior;
}
void city::RedLeave(BaseWarrior *pWarrior){
    if (pRedWarrior == pWarrior){
        pRedWarrior = NULL;
    }
}
void city::BlueEnter(BaseWarrior *pWarrior){
    pBlueWarrior = pWarrior;
}
void city::BlueLeave(BaseWarrior *pWarrior){
    if (pBlueWarrior == pWarrior){
        pBlueWarrior = NULL;
    }
}

class BaseWarrior{
    protected:
        int walksteps;
    public:
        vector<BaseWeapon * > pWeapons;
        int lifepoint, strength ;
        int location;
        int loyalty;
        int warrior_id;
        bool PosFight;
        bool NegFight;
        bool WinEnemy;
        string name, group;
        virtual void PrintBornInfo();
        virtual void SetInfo( int warrior_id_, int widx_,  const string & group_);
        virtual void SetInfo(double moral,  int warrior_id_, int widx_,  const string & group_);
        virtual void SetInfo(int royalty,  int warrior_id_, int widx_,  const string & group_);
        virtual void Attack(BaseWarrior * pWarrior);
        virtual int Hurted(int nPower);
        virtual void FightBack(BaseWarrior * pWarrior);
        virtual void LionEscape(){}
        virtual void ChangeRoyalty(){}
        virtual void GetWeapon(BaseWarrior * pWarrior){};
        void MoveTwoSteps(int steps);
        int  SwordPower();
        void ArrowShot();
        bool MightDie(BaseWarrior * enemy);
        void Killed();
        bool HaveBomb();
        int HaveArrow();
        bool HaveSword();
        int  ArrowPower(int id);
        void ArrowUsed(int id);
        void SwordUsed();
        bool Dead();
        bool AttendWar();
        void WarriorReportWeapon();
        void DeleteWeapon();
        void LifePointChange(int credit);
    
//        bool myfunction(BaseWeapon * p1, BaseWeapon * p2)
//            {return (p1->GetType() > p2->GetType());}
        BaseWarrior(){
            loyalty = 0;
            PosFight = false;
            NegFight = false;
            WinEnemy = false;
        }
        struct myclass {
            bool operator() (BaseWeapon * p1, BaseWeapon * p2) { return (p1->GetType() > p2->GetType());}
        } myobject;
        ~BaseWarrior(){
            if (pWeapons.size()>0){
                for (int i=0; i<pWeapons.size();i++){
                    BaseWeapon * p = pWeapons[i];
                    delete p;
                    cout << "Im in destruction"<<endl;
                }
            }
            pWeapons.clear();
        }
};

int city::RedPointTaken(){
    int result = 0;
    if ((pRedWarrior != NULL) && (pBlueWarrior == NULL)){
        result = Point;
        Point = 0;
        printf("%03d:%02d ", currenttime/60, currenttime%60);
       cout << "red "<< pRedWarrior->name <<" "<<pRedWarrior->warrior_id
       <<" earned "<<result<<" elements for his headquarter"<<endl;
    }
    return result;
}
int city::BluePointTaken(){
    int result = 0;
    if ((pBlueWarrior != NULL) && (pRedWarrior == NULL)){
        result = Point;
        Point = 0;
        printf("%03d:%02d ", currenttime/60, currenttime%60);
        cout << "blue "<< pBlueWarrior->name <<" "<<pBlueWarrior->warrior_id
        <<" earned "<<result<<" elements for his headquarter"<<endl;
    }
    return result;
}
void BaseWarrior::DeleteWeapon(){
            if (pWeapons.size()>0){
                for (int i=0; i<pWeapons.size();i++){
                    BaseWeapon * p = pWeapons[i];
                    delete p;
//                    cout << "Im in destruction"<<endl;
                }
            }
            pWeapons.clear();
}
void BaseWarrior::WarriorReportWeapon(){
    if(this->pWeapons.size()==0){
        cout << "no weapon" << endl;
    }
    else{
        std::sort(pWeapons.begin(), pWeapons.end(), myobject);
        for(int i=0;i< this->pWeapons.size();i++){
            if (i>0){
                cout << ",";
            }
            switch(pWeapons[i]->GetType()){
                case 2: cout << "arrow(" << this->pWeapons[i]->GetCount() << ")";
                        break;
                case 1: cout << "bomb";
                        break;
                case 0: cout << "sword(" << this->pWeapons[i]->GetPower() << ")";
                        break;
            }
        }
        cout << endl;
    }
}

void BaseWarrior::Attack(BaseWarrior * pWarrior){
    if(HaveSword()){
        int credit = pWarrior->Hurted(strength+SwordPower());
        if (credit > 0){
            LifePointChange(credit);
        }
        SwordUsed();
    }
}
int BaseWarrior::Hurted(int nPower){
    int result = lifepoint;
    lifepoint -= nPower;
    if (lifepoint < 0) { 
       lifepoint = 0;
       if (name != "lion"){
        result = 0;
       }
    }
    return result;
}
void BaseWarrior::LifePointChange(int credit){
    lifepoint += credit;
}
void BaseWarrior::FightBack(BaseWarrior * pWarrior){
    if(HaveSword()){
        pWarrior->Hurted(strength/2 + SwordPower());
        int credit = pWarrior->Hurted(strength+SwordPower());
        if (credit > 0){
            LifePointChange(credit);
        }
        SwordUsed();
    }
}
void BaseWarrior::SwordUsed(){
    for (int i=0; i<pWeapons.size(); i++){
        if (pWeapons[i]->GetType() == 0){
            pWeapons[i]->UseWeapon() ;
            if (pWeapons[i]->UseUp()){
                BaseWeapon * myWeapon = pWeapons[i];
                delete myWeapon;
                pWeapons.erase(pWeapons.begin()+i);
                i--;
            }
        }
    }
}
int BaseWarrior::SwordPower(){
    int power = 0;
    for (int i=0; i<pWeapons.size(); i++){
        if (pWeapons[i]->GetType() == 0){
            power += pWeapons[i]->GetPower();
        }
    }
    return power;
}
bool BaseWarrior::HaveSword(){
    int i = 0;
    bool found = false;
    while((!found)&&(i<pWeapons.size())){
        if(pWeapons[i]->GetType() == 0){
            found = true;
        }
        i++;
    }
    return found;    
}
bool BaseWarrior::HaveBomb(){
    int i = 0;
    bool found = false;
    while((!found)&&(i<pWeapons.size())){
        if(pWeapons[i]->GetType() == 1){
            found = true;
        }
        i++;
    }
    return found;
}
int BaseWarrior::HaveArrow(){
    int result = -1;
    for(int i = 0; i<pWeapons.size(); i++){
        if((pWeapons[i]->GetType() == 2)&&(pWeapons[i]->GetCount() > 0)){
            result = i;
            break;
        }
    }
    return result;
}
int BaseWarrior::ArrowPower(int id){
    int result = 0;
    if (id < pWeapons.size()){
        result = pWeapons[id]->GetPower();
    }
    return result;
}
void BaseWarrior::ArrowUsed(int id){
    if(id < pWeapons.size()){
        pWeapons[id]->UseWeapon();
        if (pWeapons[id]->UseUp()){
                BaseWeapon * myWeapon = pWeapons[id];
                delete myWeapon;
                pWeapons.erase(pWeapons.begin()+id);
           }
    }
}
bool BaseWarrior::MightDie(BaseWarrior * enemy){
    if ((lifepoint <= (enemy->strength + enemy->SwordPower()))
        &&(((strength+SwordPower()) < enemy->lifepoint)&&(lifepoint < (enemy->strength/2 + enemy->SwordPower())))){
        return true;
    }
    else
        return false;
}
bool BaseWarrior::AttendWar(){
    return (PosFight||NegFight);
}

bool BaseWarrior::Dead(){
    if (lifepoint <= 0) 
        return true;
    else 
        return false;
}

void BaseWarrior::MoveTwoSteps(int steps){
    if((name == "iceman" )&&(steps%2 == 0)){
        lifepoint -= 9;
        if (lifepoint<=0) 
            {lifepoint=1;}
        strength += 20;
    }
}

void BaseWarrior::PrintBornInfo(){
     printf(" %s %u born\n", name.c_str(), warrior_id);    
}

void BaseWarrior::SetInfo( int warrior_id_,  int widx_, const string & group_){
    name = WARRIOR_NAMES[widx_];
    warrior_id = warrior_id_;
    lifepoint = lifepoints[widx_];
    strength = strengths[widx_];
    group = group_;
    if(group == "red"){
        location = 0;
        cities[0].pRedWarrior = this;
    }
    else{
        location = cities.size();
        cities[num_city+1].pBlueWarrior = this;
    }
}

void BaseWarrior::SetInfo(double moral,  int warrior_id_, int widx_,  const string & group_){
}
void BaseWarrior::SetInfo(int royalty,  int warrior_id_, int widx_,  const string & group_){
}
class dragon: public BaseWarrior{
    public:
        double morale;
        void SetInfo(double morale_, int warrior_id_, int widx_,  const string & group_){
            BaseWarrior::SetInfo(warrior_id_, widx_, group_ );
            morale = morale_; 
            BaseWeapon * myWeapon = new BaseWeapon(warrior_id_%3, widx_);
            pWeapons.push_back(myWeapon);
        }        
        virtual void PrintBornInfo(){
           BaseWarrior::PrintBornInfo();
            cout << "It has a "<< pWeapons[0]->GetName() <<",and it's morale is ";
            cout << "Its morale is ";
            printf("%.2f\n", morale);            
        }
};

class ninja: public BaseWarrior{
    public:
        void SetInfo(int warrior_id_,  int widx_, const string & group_){
            BaseWarrior::SetInfo(warrior_id_,  widx_, group_);
            BaseWeapon * myWeapon = new BaseWeapon(warrior_id_%3, widx_);
            pWeapons.push_back(myWeapon);
            myWeapon = new BaseWeapon((warrior_id_+1)%3, widx_);
            pWeapons.push_back(myWeapon);
        }
        void PrintBornInfo(){
            BaseWarrior::PrintBornInfo();
            cout << "It has a "<< pWeapons[0]->GetName() <<" and a "<<pWeapons[1]->GetName() << endl;
        }
        void FightBack(BaseWarrior * pWarrior){
        
        }
};

class iceman: public BaseWarrior{
    public:
        void SetInfo(int warrior_id_,  int widx_, const string & group_){
            BaseWarrior::SetInfo(warrior_id_,  widx_, group_);
            BaseWeapon * myWeapon = new BaseWeapon(warrior_id_%3, widx_);
            pWeapons.push_back(myWeapon);
        }        
        void PrintBornInfo(){
            BaseWarrior::PrintBornInfo();
            cout << "It has a "<< pWeapons[0]->GetName() << endl;
        }
};

class lion: public BaseWarrior{
    public:
        void PrintBornInfo(){
            BaseWarrior::PrintBornInfo();
            cout << "Its loyalty is " << loyalty <<endl;
        }
        void SetInfo(int budget_, int warrior_id_, int widx_,  const string & group_){
            BaseWarrior::SetInfo( warrior_id_, widx_, group_ );
            loyalty = budget_;
        }
        void LionEscape(){
            if(loyalty <= 0){
                printf("%03d:%02d ", currenttime/60, currenttime%60);
                cout << group << " lion " << warrior_id <<" ran away"<< endl;             
                lifepoint = 0;
            }
        }
        void ChangeRoyalty(){
            
            if (AttendWar()&&(!WinEnemy)){
                loyalty -= lion_minus;
            }
        }
};


class wolf: public BaseWarrior{
    public:
        void PrintBornInfo(){
            BaseWarrior::PrintBornInfo();
        }
        void GetWeapon(BaseWarrior * pWarrior){
            if (pWeapons.size()==0){
                for(int i=0;i<pWarrior->pWeapons.size();i++){
                    pWeapons.push_back(pWarrior->pWeapons[i]);
                }
                pWarrior->pWeapons.clear();
            }
        }
};


class BaseTeam {
protected:
    int budget_;
    int warrior_id;
    int order_[MAX_WARRIORS];
    string name_;
    vector<BaseWarrior *> pTeam;
public:
    BaseTeam(int budget):budget_(budget){
        warrior_id = 1;
    }
    ~BaseTeam(){}
    void WarriorBorn();
    void ReportBudget();
//    void LionEscape();
    void BudgetChange(int value);
    void GetCityPoints();
    void RewordWarriors();
    void ClearUp();
    void ChangeRoyalty();

};
void BaseTeam::ChangeRoyalty(){
    for(int i=0;i<pTeam.size(); i++){
        pTeam[i]->ChangeRoyalty();    
    }
}
void BaseTeam::BudgetChange(int value){
    budget_ += value;
}
void BaseTeam::ReportBudget(){
    printf("%03d:%02d ", currenttime/60, currenttime%60);
    cout << budget_ << " elements in " << name_ <<" headquarter"<< endl;
}


void BaseTeam::RewordWarriors(){
    int i = 0;
    while ((i<pTeam.size())&&(budget_>=8)){
        if (pTeam[i]->WinEnemy){
            pTeam[i]->lifepoint+=8;
            budget_ -= 8;
        }    
        i++;
    }

}

void BaseTeam:: GetCityPoints(){
    for(int i=0; i<pTeam.size();i++){
        if (pTeam[i]->WinEnemy){
            budget_ += cities[pTeam[i]->location].PointTaken();
        }
    }
}

void BaseTeam:: ClearUp(){
    for(int i=0; i<pTeam.size(); i++){
        if (pTeam[i]->Dead()){
//                cout << "Im in clear up "<< endl;
                pTeam[i]->DeleteWeapon();
                BaseWarrior * mywarrior = pTeam[i];
                if(pTeam[i]->group == "red"){
                    cities[pTeam[i]->location].pRedWarrior = NULL;
                }
                else{
                    cities[pTeam[i]->location].pBlueWarrior = NULL;
                }
                delete mywarrior;
                pTeam.erase(pTeam.begin()+i);  
                i--;
        }
        else{
            pTeam[i]->WinEnemy = false;
            pTeam[i]->PosFight = false;
            pTeam[i]->NegFight = false;
        }
    }
}



void BaseTeam::WarriorBorn(){
        int widx = order_[(warrior_id-1)%5];
        if (budget_ < lifepoints[widx]) {
          return;
        }
        printf("%03d:%02d %s", currenttime/60, currenttime%60, name_.c_str());
        BaseWarrior * pWarrior;
        budget_ -= lifepoints[widx];
        if (widx==0){
           pWarrior = new dragon;
           double moral_;
           moral_ = (budget_ * 1.00)/lifepoints[0];
           pWarrior->SetInfo(moral_,  warrior_id ,widx,  name_.c_str());
        }
        else if (widx == 1){
           pWarrior = new ninja;
           pWarrior->SetInfo(warrior_id,  widx, name_.c_str());
        }
        else if (widx == 2){
           pWarrior = new iceman;
           pWarrior->SetInfo(warrior_id, widx,  name_.c_str());
        }
        else if (widx == 3){
            pWarrior = new lion ;
            pWarrior->SetInfo(budget_,  warrior_id, widx, name_.c_str());
        }
        else {
            pWarrior = new wolf ;
            pWarrior->SetInfo(warrior_id, widx, name_.c_str());
        }
       pWarrior->PrintBornInfo();    
       pTeam.push_back(pWarrior); 
       warrior_id ++; 
}

class RedTeam : public BaseTeam {
public:
    RedTeam(int budget) : BaseTeam(budget) {
      order_[0] = 2;
      order_[1] = 3;
      order_[2] = 4;
      order_[3] = 1;
      order_[4] = 0;
      name_ = "red";
    }
    ~RedTeam(){}

};



class BlueTeam : public BaseTeam {
public:
    BlueTeam(int budget) : BaseTeam(budget) {
      order_[0] = 3;
      order_[1] = 0;
      order_[2] = 1;
      order_[3] = 2;
      order_[4] = 4;
      name_ = "blue";
    }
    ~BlueTeam(){}
};



void PrintLifePoint(BaseWarrior * pWarrior){
    cout << " with " << pWarrior->lifepoint << " elements and force " << pWarrior->strength ;
}

bool MoveForward(){
    bool bWarfinished = false;
    if(cities[1].pBlueWarrior != NULL) {
        cities[1].pBlueWarrior->MoveTwoSteps(num_city+1);
        PrintTime();
        PrintSubject(cities[1].pBlueWarrior);
        cout  << " reached red headquarter" ;
        PrintLifePoint(cities[1].pBlueWarrior);
        cout<<endl;
        cities[1].pBlueWarrior->location = 0;
        if (cities[0].pBlueWarrior != NULL){
            cities[0].pExtraWarrior = cities[1].pBlueWarrior;
        }
        else{ cities[0].pBlueWarrior = cities[1].pBlueWarrior; }
        cities[1].pBlueWarrior = NULL;
     }
    for(int i=1; i<=num_city; i++){
        if(cities[i-1].pRedWarrior != NULL) {
                    cities[i-1].pRedWarrior->MoveTwoSteps(i);
                    PrintTime();
                    PrintSubject(cities[i-1].pRedWarrior);
                    cout  << " marched to city "<< i ;
                    PrintLifePoint(cities[i-1].pRedWarrior);
                    cout<<endl;
                    cities[i-1].pRedWarrior->location = i;
        }
         if(cities[i+1].pBlueWarrior != NULL) {
                    cities[i+1].pBlueWarrior->MoveTwoSteps(num_city+1-i);
                    PrintTime();
                    PrintSubject(cities[i+1].pBlueWarrior);
                    cout  << " marched to city "<< i ;
                    PrintLifePoint(cities[i+1].pBlueWarrior);
                    cout<<endl;
                    cities[i+1].pBlueWarrior->location = i;
         }
    }
    if(cities[num_city].pRedWarrior != NULL) {
        cities[num_city].pRedWarrior->MoveTwoSteps(num_city+1);
        PrintTime();
        PrintSubject(cities[num_city].pRedWarrior);
        cout  << " reached blue headquarter";
        PrintLifePoint(cities[num_city].pRedWarrior);
        cout<<endl;
        cities[num_city].pRedWarrior->location = num_city+1;
        if (cities[num_city+1].pRedWarrior != NULL){
            cities[num_city+1].pExtraWarrior = cities[num_city].pRedWarrior;
        }
        else{ cities[num_city+1].pRedWarrior = cities[num_city].pRedWarrior; }
        cities[num_city].pRedWarrior = NULL;
     }
     for(int i=2; i<=num_city+1; i++){
        if(cities[i].pBlueWarrior!=NULL){
           cities[i-1].pBlueWarrior = cities[i].pBlueWarrior;
            cities[i].pBlueWarrior = NULL;
        }
     }   
     for(int i=num_city-1; i>=0; i--){
        if(cities[i].pRedWarrior!=NULL){
            cities[i+1].pRedWarrior = cities[i].pRedWarrior;
            cities[i].pRedWarrior = NULL;
        }
     }   
     if (cities[0].pExtraWarrior!=NULL){
            PrintTime();
            cout << "red headquarter was taken" << endl;
            bWarfinished = true;
     }
     if (cities[num_city+1].pExtraWarrior!=NULL){
            PrintTime();
            cout << "blue headquarter was taken" << endl;
            bWarfinished = true;     
     }
     return  bWarfinished;
}

void BombShot(){
    for(int i=1; i<=num_city; i++){
        if(cities[i].pRedWarrior != NULL) {
            if(cities[i].pRedWarrior->HaveBomb() && (cities[i].pBlueWarrior != NULL)){
                if(cities[i].pRedWarrior->MightDie(cities[i].pBlueWarrior)){
                    PrintTime();
                    PrintSubject(cities[i].pRedWarrior);
                    cout  << " used a bomb and killed ";
                    PrintSubject(cities[i].pBlueWarrior);
                    cout<<endl;
                    cities[i].pRedWarrior->lifepoint = 0;
                    cities[i].pBlueWarrior->lifepoint = 0;
                }
            }
        }
         if(cities[i].pBlueWarrior != NULL) {
            if(cities[i].pBlueWarrior->HaveBomb() && (cities[i].pRedWarrior != NULL)){
                if(cities[i].pBlueWarrior->MightDie(cities[i].pRedWarrior)){
                    PrintTime();
                    PrintSubject(cities[i].pBlueWarrior);
                    cout  << " used a bomb and killed ";
                    PrintSubject(cities[i].pRedWarrior);
                    cout<<endl;
                    cities[i].pRedWarrior->lifepoint = 0;
                    cities[i].pBlueWarrior->lifepoint = 0;
                }
            }
        }           
    }
}

void ArrowShot(){
    for(int i=0; i<=num_city+1; i++){
        if((cities[i].pRedWarrior != NULL) && (i<num_city)){
 //           cout  << " im here " << endl ; 
            int nArrow = cities[i].pRedWarrior->HaveArrow();
            if( (nArrow >= 0 ) && (cities[i+1].pBlueWarrior != NULL)){
                cities[i+1].pBlueWarrior->Hurted( cities[i].pRedWarrior->ArrowPower(nArrow));
                cities[i].pRedWarrior->ArrowUsed(nArrow);
                PrintTime();
                PrintSubject(cities[i].pRedWarrior);
                cout  << " shot" ;
                if (cities[i+1].pBlueWarrior->Dead()){
                      cout  <<" and killed ";
                      PrintSubject(cities[i+1].pBlueWarrior);
                }
               cout<<endl;
            }
        }
        
        if((cities[i].pBlueWarrior != NULL) && (i>1)){
//            cout  << " im there " << endl ;
           int nArrow = cities[i].pBlueWarrior->HaveArrow(); 
           if( (nArrow>=0) && (cities[i-1].pRedWarrior != NULL)){
                cities[i-1].pRedWarrior->Hurted(cities[i].pBlueWarrior->ArrowPower(nArrow));
//                cout  << " im ******* " << endl ;
                cities[i].pBlueWarrior->ArrowUsed(nArrow);
                PrintTime();
                PrintSubject(cities[i].pBlueWarrior);
                cout  << " shot";
                if (cities[i-1].pRedWarrior->Dead()){
                      cout  <<" and killed ";
                      PrintSubject(cities[i-1].pRedWarrior);
                }
               cout<<endl;
            }
        }
    }
}
void PrintTime(){
    printf("%03d:%02d ", currenttime/60, currenttime%60);
}
void PrintSubject(BaseWarrior * pWarrior){
    cout << pWarrior->group << " " << pWarrior->name << " " << pWarrior->warrior_id;
}
void WarriorFight(){
    for(int i=1; i<=num_city; i++){   // first fight
 //       cout << "part 1"<< endl;
        if((cities[i].pRedWarrior!=NULL)&&(cities[i].pBlueWarrior!=NULL)){
            if((!cities[i].pRedWarrior->Dead())&&(!cities[i].pBlueWarrior->Dead())){
                if((cities[i].flag == 1)
                ||((cities[i].flag == 0)&&(i%2 == 1))){
                    cities[i].pRedWarrior->PosFight = true;
                    cities[i].pBlueWarrior->NegFight = true;
                    cities[i].pRedWarrior->Attack(cities[i].pBlueWarrior);
                    PrintSubject(cities[i].pRedWarrior);
                    cout << " attacked at city " << i ;
                    PrintSubject(cities[i].pBlueWarrior);
                    cout << endl;
                    
                }
                else{ 
                    cities[i].pRedWarrior->NegFight = true;
                    cities[i].pBlueWarrior->PosFight = true;
                    cities[i].pBlueWarrior->Attack(cities[i].pRedWarrior);
                    PrintSubject(cities[i].pBlueWarrior);
                    cout << " attacked at city " << i ;
                    PrintSubject(cities[i].pRedWarrior);
                    cout << endl;
                }
            }
        }
    }
    for(int i=1; i<=num_city; i++){ // first back
//        cout << "part 2"<< endl;
        if((cities[i].pRedWarrior!=NULL)&&(cities[i].pBlueWarrior!=NULL)){
            if((!cities[i].pRedWarrior->Dead())&&(!cities[i].pBlueWarrior->Dead())){
                if((cities[i].flag == 1)
                ||((cities[i].flag == 0)&&(i%2 == 1))){
                    cities[i].pBlueWarrior->FightBack(cities[i].pRedWarrior);
                    PrintSubject(cities[i].pBlueWarrior);     /////////
                    cout << " FightBacked at city " << i ;                        /////////
                    PrintSubject(cities[i].pRedWarrior);        /////////
                    cout << endl;                               /////////
                }
                else{ 
                    cities[i].pRedWarrior->FightBack(cities[i].pBlueWarrior);
                    PrintSubject(cities[i].pRedWarrior);     /////////
                    cout << " FightBacked at city " << i ;                       /////////
                    PrintSubject(cities[i].pBlueWarrior);        /////////
                    cout << endl;                               /////////
               }
            }
        }
    }
    for(int i=1; i<=num_city; i++){  // print out killed info
//        cout << "part 3"<< endl;
        if((cities[i].pRedWarrior!=NULL)&&(cities[i].pBlueWarrior!=NULL)){
//            if((cities[i].pRedWarrior->Dead())&&(cities[i].pRedWarrior->AttendWar())&&(!cities[i].pBlueWarrior->Dead())){
            if((cities[i].pRedWarrior->Dead())&&(!cities[i].pBlueWarrior->Dead())){
                cities[i].pBlueWarrior->WinEnemy = true;
                cities[i].pBlueWarrior->GetWeapon(cities[i].pRedWarrior);
                cities[i].CityWarResult(BLUE_WIN);
                PrintTime();
                PrintSubject(cities[i].pRedWarrior);
                cout << " was killed in city "<< i << endl;
             }
//            else if ((cities[i].pBlueWarrior->Dead())&&(cities[i].pBlueWarrior->AttendWar())&&(!cities[i].pRedWarrior->Dead())){ 
            else if ((cities[i].pBlueWarrior->Dead())&&(!cities[i].pRedWarrior->Dead())){ 
                cities[i].pRedWarrior->WinEnemy = true;
                cities[i].pRedWarrior->GetWeapon(cities[i].pBlueWarrior);
                cities[i].CityWarResult(RED_WIN);
                PrintTime();
                PrintSubject(cities[i].pBlueWarrior);
                cout << " was killed in city "<< i << endl;
            }
            else{
                cities[i].CityWarResult(TIE);
            }
        }
    }
    DragonYell();
}
void DragonYell(){
     for(int i=1; i<=num_city; i++){
        if (cities[i].pRedWarrior!=NULL){
            if(cities[i].pRedWarrior->name == "dragon"){
                dragon *pDragon = reinterpret_cast<dragon *>(cities[i].pRedWarrior);
                if ((cities[i].pRedWarrior->PosFight)&&(cities[i].pRedWarrior->lifepoint > 0)){
                    if (cities[i].pBlueWarrior->lifepoint < 0){
                        pDragon->morale += 0.2;
                    }
                    else if (cities[i].pBlueWarrior->lifepoint > 0){
                        pDragon->morale -= 0.2;
                    }
                    if (pDragon->morale > 0.8){
                        PrintTime();
                        PrintSubject(cities[i].pRedWarrior);
                        cout << " yelled in city "<< i << endl;
                    }
                }
            }
        }
         if (cities[i].pBlueWarrior!=NULL){
            if(cities[i].pBlueWarrior->name == "dragon"){
                dragon *pDragon = reinterpret_cast<dragon *>(cities[i].pBlueWarrior);
                if ((cities[i].pBlueWarrior->PosFight)&&(cities[i].pBlueWarrior->lifepoint > 0)){
                    if (cities[i].pRedWarrior->lifepoint < 0){
                        pDragon->morale += 0.2;
                    }
                    else if (cities[i].pRedWarrior->lifepoint > 0){
                        pDragon->morale -= 0.2;
                    }
                    if (pDragon->morale > 0.8){
                        PrintTime();
                        PrintSubject(cities[i].pBlueWarrior);
                        cout << " yelled in city "<< i << endl;
                    }
                }
            }
        }
   }   
}
void ReportWeapon(){
    for(int i=0;i<=num_city+1;i++){
        if(cities[i].pRedWarrior!=NULL){
            PrintTime();
            PrintSubject(cities[i].pRedWarrior);
            cout << " has " ;
            cities[i].pRedWarrior->WarriorReportWeapon();
        }
    }
    for(int i=0;i<=num_city+1;i++){
        if(cities[i].pBlueWarrior!=NULL){
            PrintTime();
            PrintSubject(cities[i].pBlueWarrior);
            cout << " has " ;
            cities[i].pBlueWarrior->WarriorReportWeapon();
        }
    }
}
void  LionEscape(){
//    cout << "escape all " << endl;
    if(cities[0].pRedWarrior!=NULL){
        cities[0].pRedWarrior->LionEscape();
    }
    for(int i=1; i<=num_city; i++){
        if(cities[i].pRedWarrior!=NULL){
            cities[i].pRedWarrior->LionEscape();
        }
        if(cities[i].pBlueWarrior!=NULL){
            cities[i].pBlueWarrior->LionEscape();
        }
    }
    if(cities[num_city+1].pBlueWarrior!=NULL){
        cities[num_city+1].pBlueWarrior->LionEscape();
    }
}

int main(void) {
    int num_case;
    int budgets;
    int time;
    int min;
    bool warfinish;
    
    cin >> num_case;
    for ( int c = 0; c < num_case; ++c ) {
        cin >> budgets >> num_city >> arrow_power >> lion_minus >> time ;
        for ( int w = 0; w < MAX_WARRIORS; ++w ) {
          int cst;
          cin >> cst;
          lifepoints.push_back(cst);
        }
        for ( int w = 0; w < MAX_WARRIORS; ++w ) {
          int pwrs;
          cin >> pwrs;
          strengths.push_back(pwrs);
        }
        warfinish = false;
        for(int i=0; i<=num_city+1; i++){
             city mycity(i);
             cities.push_back(mycity);
        }
        printf("Case:%u\n" ,c+1);
        RedTeam red = RedTeam(budgets);
        BlueTeam blue = BlueTeam(budgets);
        while((!warfinish)&&(currenttime<time)){
            min = currenttime % 60;
            if( min== 0){
                red.WarriorBorn();
                blue.WarriorBorn();
            }
            else if (min == 5){
                LionEscape();
                red.ClearUp();
                blue.ClearUp();
            } 
            else if (min == 10){
                warfinish = MoveForward();
            }
            else if (min == 20){
//                cout << "20" << endl;
                for(int i=1; i<=num_city; i++){
                    cities[i].CreatePoint();
                }
            }
            else if (min == 30){
//                cout << "30" << endl;
                for(int i=1; i<=num_city; i++){
                    red.BudgetChange(cities[i].RedPointTaken());
                    blue.BudgetChange(cities[i].BluePointTaken());
                }
            }
            else if (min == 35){
//               ArrowShot();
            }
            else if (min == 38){
//                BombShot();
//                red.ClearUp();
//                blue.ClearUp();
            }
            else if (min == 40){
//                cout << "40" << endl;
                WarriorFight();
//                cout << "fight over" << endl;
                
                red.ChangeRoyalty();
                blue.ChangeRoyalty();
                red.RewordWarriors();
                blue.RewordWarriors();
                red.GetCityPoints();
                blue.GetCityPoints();
                for(int i = 1; i<=num_city; i++){
                    cities[i].ChangeFlags();
                }
                red.ClearUp();
                blue.ClearUp();
           }
            else if (min == 50){
//                cout << "50" << endl;
                red.ReportBudget();
                blue.ReportBudget();
            }
            else if (min == 55){
                ReportWeapon();
            }
            currenttime++;
        }
    }
    return 1;
}

