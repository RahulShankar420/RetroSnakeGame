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