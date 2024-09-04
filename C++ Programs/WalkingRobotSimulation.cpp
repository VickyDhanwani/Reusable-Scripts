/*
class Solution {
public:
    int final_distance;
    int first_move;
    // North = 0. east = 1, south = 2, west = 3
    // if turn right, new dir = dir  + 1
    // if turn left new dir = dir - 1
    int newDir(int d, int t) {
        if(t == -1) {
            return (d + 1) % 4;
        }
        if(t == -2 && d == 0) return 3;
        return (d - 1) % 4;
    }

    pair<int, int> newPosition(pair<int,int> cp, int dir,  map<pair<int,int>, int> mp, int val ) {
        if(dir == 0) {
            for(int i = cp.second; i < cp.second + val; i++) {
                
                if(mp[make_pair(cp.first, i)] == 1) {
                    if(first_move == 0) { 
                        first_move = 1;
                        continue;
                    } 
                    i--;
                   
                    return make_pair(cp.first, i);
                    break;
                }
                first_move = 1;
                
            }
            return make_pair(cp.first, cp.second + val);
            cout<<endl;

        }
        else if(dir == 1) {
            for(int i = cp.first; i < cp.first + val; i++) {
                cout<<i<<"->";
                
                if(mp[make_pair(i, cp.second)] == 1) {
                    if(first_move == 0) { 
                        first_move = 1;
                        continue;
                    } 
                    i--;
                    return make_pair(i, cp.second);
                    break;
                }
                first_move = 1;
            }
            cout<<endl;
            return make_pair(cp.first + val, cp.second );
            
        }
        else if(dir == 2) {
            int i = cp.second;
            while(i > cp.second - val) {
                if(mp[make_pair(cp.first, i)] == 1) {
                    if(first_move == 0) { 
                        first_move = 1;
                        i--;
                        continue;
                    } 
                    i++;
                    break;
                }
                first_move = 1;
                i--;
            }
            return make_pair(cp.first, i);
        }
        int i = cp.first;
            while(i > cp.first - val) {
                if(mp[make_pair(i, cp.second)] == 1) {
                    if(first_move == 0) { 
                        first_move = 1;
                        i--;
                        continue;
                    } 
                    i++;
                    break;
                }
                first_move = 1;
                i--;
            }
            return make_pair(i, cp.second);
    }
    int robotSim(vector<int>& commands, vector<vector<int>>& obstacles) {
        pair<int,int> curr_pos;
        curr_pos.first = 0;
        curr_pos.second = 0;
        int dir = 0;
        final_distance = 0;
        first_move = 0;
        map<pair<int,int>, int> mp;
        for(int i = 0; i < obstacles.size(); i++) {
            mp[make_pair(obstacles[i][0], obstacles[i][1])] = 1;
        }
     
        for(int cmd = 0; cmd < commands.size(); cmd++) {
            if(commands[cmd] == -1) {
                dir = newDir(dir, -1);
                cout<<"Direction Changed to "<<dir<<endl;
            }
            else if (commands[cmd] == -2) {
                dir = newDir(dir, -2);
                cout<<"Direction Changed to "<<dir<<endl;
            }
            else {
                curr_pos = newPosition(curr_pos, dir, mp, commands[cmd] );
                cout<<"New Position is : "<<curr_pos.first<<" "<<curr_pos.second<<endl;
                int distance = pow(abs(curr_pos.first), 2) + pow(abs(curr_pos.second), 2);
                if(distance > final_distance) {
                    final_distance = distance;
                }
            }
        }
        int distance = pow(abs(curr_pos.first), 2) + pow(abs(curr_pos.second), 2);

        return final_distance;

    }
};
*/
class Solution {
public:
    int robotSim(vector<int>& commands, vector<vector<int>>& obstacles) {
        // Directions: North, East, South, West
        const vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        unordered_set<pair<int, int>, hash_pair> obstacleSet;
        
        // Populate the obstacle set
        for (const auto& obstacle : obstacles) {
            obstacleSet.insert({obstacle[0], obstacle[1]});
        }
        
        int x = 0, y = 0; // Robot's current position
        int dir = 0; // Initial direction is North
        int maxDistSq = 0; // Maximum Euclidean distance squared
        
        for (const int& command : commands) {
            if (command == -1) {
                // Turn right
                dir = (dir + 1) % 4;
            } else if (command == -2) {
                // Turn left
                dir = (dir + 3) % 4; // +3 is equivalent to -1 in mod 4
            } else {
                // Move forward
                for (int i = 0; i < command; ++i) {
                    int newX = x + directions[dir].first;
                    int newY = y + directions[dir].second;
                    
                    // Check for obstacles
                    if (obstacleSet.count({newX, newY}) == 0) {
                        x = newX;
                        y = newY;
                        // Update max distance squared
                        int distSq = x * x + y * y;
                        maxDistSq = max(maxDistSq, distSq);
                    }
                    else {
                        break;
                    }
                }
            }
        }
        
        return maxDistSq;
    }
    
private:
    // Custom hash function for pair<int, int>
    struct hash_pair {
        template <class T1, class T2>
        std::size_t operator () (const std::pair<T1, T2>& p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            return h1 ^ h2; // Combine the hash of the two values
        }
    };
};
