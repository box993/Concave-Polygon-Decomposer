#include <bits/stdc++.h>
#include <sys/stat.h>
using namespace std;

class HalfEdge;
class Vertex;
class Face;
class DCEL;

/// @brief Directed edge of a DCEL.
/// This represents an edge originating from the origin Vertex.
/// @param origin This Vertex is the origin of the half-edge
/// @param twin This half edge represents the equivalent edge in the opposite direction which is inbound on the origin
/// @param face This represents the face associated with this half-edge
/// @param next This represents the half-edge that follows the current half-edge
class HalfEdge {
public:
    Vertex* origin;
    HalfEdge* twin;
    Face* face;
    HalfEdge* next;

    /// @brief Constructor of the HalfEdge class. It sets everything to NULL
    HalfEdge() : origin(nullptr), twin(nullptr), face(nullptr), next(nullptr){}
};

/// @brief Point in the DCEL.
/// This class represents a singular point in the DCEL. 
/// @param x X-coordinate of the vertex
/// @param y y-coordinate of the vertex
/// @param incidentEdge The HalfEdge originating from the current vertex
/// @param prev The HalfEdge originating from the current vertex but in the opposite direction
class Vertex {
public:
    double x;
    double y;
    HalfEdge* incidentEdge;
    HalfEdge* prev; //danger, works only on face 0

    /// @brief This function calculates the x value of the coordinate of intersection of the line passing through v1,v2 and the line passing through the current vertice, parallel to the x axis.
    /// @param v1 This is the first Vertex of the line
    /// @param v2 This is the second Vertex of the line
    /// @return Returns the x value of the point of intersection of the line passing through v1,v2 and the line passing through the current vertice, parallel to the x axis.
    double intersection(Vertex* v1, Vertex* v2);

    /// @brief This function returns if the Vertex calling the function is on the same side of the line created by v1 and v2 as last[Lm]
    /// @param v1 First Vertex of the line
    /// @param v2 Second Vertex of the line
    /// @param val Val is the value returned when you evaluate last[lm] on the line created by v1 and v1
    /// @return Returns a boolean value of whether the point is on the correct side or not
    bool side(Vertex* v1, Vertex* v2, int val);

    /// @brief This function checks if the vertex calling is inside the rectangle defined by x1, x2, y1, y2
    /// @param x1 Vertex of the rectangle
    /// @param x2 Vertex of the rectangle
    /// @param y1 Vertex of the rectangle
    /// @param y2 Vertex of the rectangle
    /// @return Returns true if the vertex is inside the rectangle
    bool insideRect(double x1, double x2, double y1, double y2);

    /// @brief Checks if the calling vertex is inside the polygon created with st as first vertex and ed as last vertex. It Uses the ray casting algorithm to find if the point is inside the polygon or not.
    /// @param st starting Vertex of the polygon
    /// @param ed ending Vertex of the polygon
    /// @return true if the calling vertex is inside the polygon.
    bool inside(Vertex* st, Vertex* ed);

    /// @brief Finds and returns the next Vertex
    /// @return Returns the next Vertex
    Vertex* next();

    Vertex(double x_, double y_) : x(x_), y(y_), incidentEdge(nullptr) {}
    Vertex(double x_, double y_, HalfEdge* incidentEdge_) : x(x_), y(y_), incidentEdge(incidentEdge_) {}
};

/// @brief Class to represent the Face of a polygon.
/// This class represents the Face of a polygon
/// @param outerComponent The HalfEdge that we use to represent our face
class Face {
public:
    HalfEdge* outerComponent;
    Face() : outerComponent(nullptr) {}

    /// @brief We use this function to get the previous Vertex within a face
    /// @param v We get the Vertex that is the previous of v along the edges of the face calling this function.
    /// @return Thre preivious vertex along the face
    Vertex* prev(Vertex* v);
};

/// @brief Class to represent a doubly connected edge list. This is the class we use to decompose the polygon.
/// This class represents our implimentation of the doubly connected edge list. 
/// @param vertices This is a list of all of the vertices in the dcel
/// @param halfEdges This is a list of all halfedges in the dcel
/// @param faces This is a list of all faces in the dcel
/// @param diags This is the list of all the new edges that we add to the polygon in-order to decompose it
/// @param mp We use this map to link an integer i to the face of the ith polygon that we have decomposed
/// @param mp1 We use this map to link a Face to its assigned integer
/// @param LDP We use this vector to keep track if the ith face is part of the final decomposed polygon or not. We use mp and mp1 inorder to get to index of the face we want to check.
class DCEL {
public:
    vector<Vertex*> vertices;
    vector<HalfEdge*> halfEdges;
    vector<Face*> faces;
    vector<HalfEdge*> diags;
    map<int,Face*> mp;
    map<Face*,int> mp1;
    vector<bool> LDP;
    DCEL(vector<Vertex*>& vertices);

    /// @brief This function splits the Face into two faces or two plygons using v1 and v2 as the diagonal
    /// @param v1 One end of the diagonal with which we want to split the Face
    /// @param v2 One end of the diagonal with which we want to split the Face
    void split(Vertex*v1, Vertex* v2);

    /// @brief This function implements algorithm 1 from the paper
    void algorithm1();

    /// @brief This function implements the merging algorithm from the paper
    void merging();
};

Vertex* Face::prev(Vertex* v){
    HalfEdge* e = this->outerComponent;
    while(e->next->origin!= v){ // We go through all the halfedges of the face until we reach the previous vertex
        e = e->next;
    }
    return e->origin;
}

void DCEL::split(Vertex*v1, Vertex* v2){
    HalfEdge* one = new HalfEdge(); // we create the two halfedges of the diagonal
    HalfEdge* two = new HalfEdge();
    this->diags.push_back(one); // we include this diagonal into diags for when we merge later
    one->twin = two;
    two->twin = one;
    one->origin = v1;
    two->origin = v2; // we initialise the origins of the half edge diagonal and its twin

    Face* small = new Face(); // we create a new face 
    small->outerComponent = two; // assign two to the new face
    this->faces[0]->outerComponent = one; // assign one to the existing face

    HalfEdge* e = v1->incidentEdge;
    while(e->origin!=v2) // assign all the edges of the new face to 'small'
    {
        e->face = small;
        e = e->next;
    }
    two->face = small; //assign one and two to its respective faces
    one->face = this->faces[0];
    
    one->next = v2->incidentEdge;  // reconnect the edges adjacent to the diagonal to their correct next edges
    two->next = v1->incidentEdge;
    v2->prev->twin->next = two;   
    v1->prev->twin->next = one;
    v2->prev = two;

    v1->incidentEdge = one;
    this->faces.push_back(small);

    mp[this->faces.size()-1] = small; // we map small to its index and vice versa
    mp1[small] = this->faces.size()-1;
}

DCEL::DCEL(vector<Vertex*>& inp) {
    int n = inp.size();
    if (n < 3) {
        std::cerr << "Error: cannot create DCEL for a polygon with less than 3 vertices\n";
        return;
    }
    this->vertices = inp;
    for (int i = 0; i < n; i++) { // we make all the halfedge connections

        HalfEdge* he1 = new HalfEdge();
        he1->origin = this->vertices[i];
        this->halfEdges.push_back(he1);
        this->vertices[i]->incidentEdge = he1;
    }

    for (int i = 0; i < n; i++) { //we create twin half edges

        HalfEdge* he1 = new HalfEdge();
        he1->origin = this->vertices[(n-i+1)%n];
        this->halfEdges.push_back(he1);
    }

    for (int i = 0; i < n; i++) { //assign twins
        this->halfEdges[i]->next = this->halfEdges[(i + 1) % n];
        this->halfEdges[i]->twin = this->halfEdges[i + n];
        this->halfEdges[i]->next->origin->prev = this->halfEdges[i]->twin;
        this->halfEdges[i+n]->twin = this->halfEdges[i];
    }

    for (int i = n; i < 2*n -1; i++) {
        this->halfEdges[i]->next = this->halfEdges[i + 1]; //assign next
    }

    this->halfEdges[2*n - 1]->next = this->halfEdges[n];

    Face* face = new Face(); //create new face, assign respective values to it
    this->faces.push_back(face);
    mp[0] = face;
    mp1[face] = 0;

    HalfEdge* he = this->halfEdges[0];
    do {
        he->face = face;
        he = he->next;
    } while (he != this->halfEdges[0]);

    face->outerComponent = halfEdges[0];
}

bool signedArea(Vertex* v0, Vertex* v1, Vertex* v2){ //calculates signed area, return true if v0 v1 v2 form reflex angle

    if(((v1->x - v0->x)*(v2->y - v0->y) - (v2->x - v0->x)*(v1->y - v0->y)) >= 0)
        return true;
    return false;
}

double Vertex::intersection(Vertex* v1, Vertex* v2){ //returns x coord of intersection of v1v2 and horizontal line through vertex
    
    return v1->x + (this->y - v1->y) * (v2->x - v1->x) / (v2->y - v1->y);
}

bool Vertex::side(Vertex* v1, Vertex* v2, int val){ //checks if Vertex is on same side as Last[Lm] wrt v1v2 line
    int sign; //sign of the value returned when last[lm] is evaluated on v1v2's line
    if(val<0) sign = -1;
    else if(val>0) sign = 1;
    else return true;
    if(((this->y-v1->y)*(v2->x - v1->x) - (v2->y - v1->y)*(this->x - v1->x)) * sign >= 0) return true;
    return false;
}

bool Vertex::inside(Vertex* st, Vertex* ed){
    int counter = 0; //counter of the number of lines a ray from 'this' parallel to x-axis in positive direction intersects
    double intersect = -10000.0;
    HalfEdge* e = st->incidentEdge, *start = e;
    do{
        Vertex* v1 = e->origin, *v2 = v1->next();
        if(v1->y == v2->y) // edge case of if an edge of the face is parallel to the x-axis
        {
            if(this->y == v1->y){
                if(this->x < min(v1->x, v2->x)) counter++; //vertex is to the left of this edge and intersects it
                else if(this->x >= min(v1->x,v2->x) and this->x <= max(v1->x,v2->x)) return true; // vertex is on the line segment of the face
            }
            e = e->next;
            continue;
        }
        intersect = this->intersection(v1,v2);
        if(this->x == intersect) return true; // another case where the vertex is on the line segment
        if(max(v1->y,v2->y) >= this->y and min(v1->y,v2->y) < this->y and this->x < intersect) // case where the ray crosses the line-segment
        {
            counter++;
        }
        e = e->next;
    }while(e->origin != ed);

    Vertex* v1 = st, *v2 = ed;
    if(v1->y == v2->y and this->y == v1->y) // for the final edge of the face
    {
        if(this->x < min(v1->x, v2->x)) counter++;
        else if(this->x >= min(v1->x,v2->x) and this->x <= max(v1->x,v2->x)) return true;
    }
    intersect = this->intersection(v1,v2);
    if(this->x == intersect) return true;

    if(max(v1->y,v2->y) >= this->y and min(v1->y,v2->y) < this->y and this->x < intersect){
        counter++;
    }
    return (counter%2 == 1);
}

bool Vertex::insideRect(double x1, double x2, double y1, double y2){
    if(this->x >= x1 and this->x <= x2 and this->y >= y1 and this->y <= y2) return true;
    return false;
}

Vertex* Vertex::next(){
    return this->incidentEdge->next->origin;
}

void DCEL::algorithm1()
{
    int n = this->vertices.size();
    vector<Vertex*> v = this->vertices;
    vector<vector<Vertex*>> L(n*100);
    L[0].push_back(v[0]); // we push the first vertex into L[0]
    int m = 1;
    while(n > 3){
        v.clear();
        v.push_back(L[m-1].back()); 
        v.push_back(v[0]->next());
        L[m].push_back(v[0]); //L[m] contains the potential new polygon. We add the last element of L[m-1] as the first element of L[m]
        L[m].push_back(v[1]);
        
        int i = 1;
        v.push_back(v[i]->next());  // we greedily add points to v as long as it does not create a notch
        while(L[m].size() < n and 
                signedArea(v[i-1], v[i], v[i+1]) and 
                    signedArea(v[i], v[i+1], v[0]) and 
                        signedArea(v[i+1], v[0], v[1])) // we check if the addition of the i+1th point creates a notch
        {
            L[m].push_back(v[i+1]);
            i++;
            v.push_back(v[i]->next());        
        }

        if(L[m].size() != n)
        {
            queue<Vertex*> LPVS;    //list that contains all elements in the undecomposed polygon that arent in L[m]
            HalfEdge* e = this->faces[0]->outerComponent, *start = this->faces[0]->outerComponent;
            Vertex* vertice;
            do{
                vertice = e->origin;
                bool check = false;
                for(auto j: L[m]){
                    if(j==vertice){
                        check = true;
                        break;
                    }
                }
                if(!check) LPVS.push(vertice);
                e = e->next;
            }while(e != start);

            while(LPVS.size()>0 and L[m].size()>2) //loop until LPVS is empty
            {
                double x1=1e5,x2=-1e5,y1=1e5,y2=-1e5; // we calculate the boundaries of the rectangle here
                for(auto vertice: L[m]){
                    x1 = min(x1,vertice->x);
                    x2 = max(x2,vertice->x);
                    y1 = min(y1,vertice->y);
                    y2 = max(y2,vertice->y);
                }
                bool backward = false;

                while(!backward and LPVS.size()>0){
                    Vertex* V;
                    while(true) // for every point in LPVS we check if it is inside the recatangle, if it is, we check if it is inside the polygon
                    {

                        if(LPVS.size()==0) break;
                        V = LPVS.front();
                        bool isInside = V->insideRect(x1,x2,y1,y2);
                        if(isInside) break;
                        else LPVS.pop();
                    }
                    if(LPVS.size()>0)
                    {
                        Vertex* fi = L[m].front(), *la = L[m].back();
                        if(V->inside(fi, la)) // V is inside our current polygon so we need to remove vertices
                        {
                            Vertex* last = L[m].back();
                            int val = (last->y-v[0]->y)*(V->x - v[0]->x) - (V->y - v[0]->y)*(last->x - v[0]->x);
                            
                            if(val==0)
                            {
                                L[m] = {L[m].front(), L[m].front()->next()};
                                break;
                            }
                            else{
                                vector<Vertex*> VTR; // list of vertices we want in Lm
                                for(auto vertice: L[m]){
                                    if(vertice == L[m].front()){
                                        VTR.push_back(vertice);
                                        continue;
                                    }
                                    if(!vertice->side(v[0], V, val)) // we check if vertice is on same side as last[Lm] wrt line v0-V
                                        VTR.push_back(vertice); //if no, we push it to VTR
                                }
                                L[m] = VTR; //reassign L[m] with elements not on the same side of last[Lm]
                            }
                            backward = true; //asks us to backtrack the vertices of L[m]
                        }
                        LPVS.pop();
                    }
                }
            }
        }

        if(L[m].back() != v[1]){
            if(L[m].back()->next() != L[m].front()) {
                this->split(L[m].front(), L[m].back()); //if new polygon in L[m] is valid, split the parent face based on the new polygon
            }
            n = n-L[m].size() + 2;
        }
        m++;
    }
}

void DCEL::merging()
{
    int m = this->diags.size();
    int np = m+1;
    this->LDP.resize(np,true); // LDP is an array indication if the ith face is part of the decomposition or not

    Vertex *Vs, *Vt;
    for(int j = 0; j < m; j++) // we loop through every diagonal in the decomposition
    {
        Vt = this->diags[j]->origin;
        Vs = this->diags[j]->next->origin;

        Vertex *j2 = Vt;
        Vertex *i2 = Vs;
        Vertex *i1 = this->diags[j]->next->next->origin;
        Vertex* j1 = this->diags[j]->twin->next->next->origin; 
        Vertex* i3 = this->diags[j]->twin->face->prev(Vs);
        Vertex *j3= this->diags[j]->face->prev(Vt); // we get the vertices surrounding the diagonal on either side respectively
        
        bool x = !signedArea(i1, i2, i3), y = !signedArea(j1, j2, j3); // we check if the removal of the diagonal will create a notch

        if( x && y ) // if no notch will be created, we merge
        {
            np++;
            Face* cur = new Face(); // we create a new face
            HalfEdge* e = this->diags[j]->next;
            cur->outerComponent = e;
            HalfEdge* hold;
            do{
                e->face = cur;
                e = e->next;
                if(e->next->origin==Vt){
                    hold = e; // stores the edge just before vt
                }
            }while(e->origin != Vt); // we assign the new face to every edge in the old face till vt

            hold->next = this->diags[j]->twin->next; // we make a new connection skipping the diagonal

            e = this->diags[j]->twin->next;
            do{
                e->face = cur;
                e = e->next;
                if(e->next->origin==Vs){
                    hold = e;
                }
            }while(e->origin != Vs); // we assign the remaining edges to the new face
            hold->next = this->diags[j]->next;

            this->faces.push_back(cur);
            this->mp[np-1] = cur;
            this->mp1[cur] = np-1; // we add the new face and its index to the map

            this->LDP.push_back(true);
            this->LDP[mp1[this->diags[j]->face]] = false;
            this->LDP[mp1[this->diags[j]->twin->face]] = false; // we update the LDP array
        }
    }
}

int main(){
    /**
     * @brief This is the main function.
     * We open and loop through the input folders and create respective output folders
     * For each input we read, we start a timer, create the DCEL, and call the functions algorithmi(); and merging();
     * Then we print the result into the output file
     */

    //PRITHVI RAJAN 2020A7PS2080H
    //PRAJWAL NAYAK 2020A7PS2059H
    //MEDINI N B 2020A7PS1722H

    mkdir("./decomposed", 0777);

    //below for loop is for n folders that we used for testing
    for(int n=8; n<=30; n++) {

        string input_folder = "./polygons_input/" + to_string(n) + "/";
        string output_folder = "./decomposed/" + to_string(n) + "/";

        int result = mkdir(output_folder.c_str(), 0777); // create output folder if it doesn't exist

        if (result == -1) {
            perror("Error creating decomposed directory");
            return 1;
        }

        //below for loop is for reading 10 input text files that we used for testing
        for(int i=1; i<=10; i++) {

            string input_file = input_folder + to_string(n) + "_" + to_string(i) + ".txt";
            string output_file = output_folder + to_string(n) + "_" + to_string(i) + ".txt";

            //freopen("input.txt", "r", stdin);
            //freopen("output.txt", "w", stdout);
            freopen(input_file.c_str(), "r", stdin);
            freopen(output_file.c_str(), "w", stdout);


            struct timespec st1={0,0}, st2={0,0};
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &st1);
            vector<Vertex*> inp;
            int siz;cin>>siz;

            for (int i = 0; i < siz; ++i)
            {
                double x,y;cin>>x>>y;
                inp.push_back(new Vertex(x,y));
            }
            //reverse(inp.begin(), inp.end()); //for clockwise, uncomment this line

            DCEL* polygon = new DCEL(inp);
            polygon->algorithm1();
            polygon->merging();
  
            for(auto poly: polygon->faces){
                if(!polygon->LDP[polygon->mp1[poly]]) continue;
                HalfEdge* e = poly->outerComponent, *start = e;
                do{
                    cout<<e->origin->x<<", "<<e->origin->y<<endl;
                    e = e->next;
                } while(e!=start);
                cout<<endl;
            }
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &st2);
            double tim = (((double)st2.tv_sec*1.0e9 + st2.tv_nsec) - ((double)st1.tv_sec*1.0e9 + st1.tv_nsec))/1.0e9;
            cout<<tim<<endl;
        }
    }
}