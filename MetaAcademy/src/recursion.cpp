/*
 * author: Mohit Chawla | mc2683@cornell.edu | Cornell Tech Connective Media
 * Program
 * decription:CS106b-assignment-3-meta-accademy-http://web.stanford.edu/class/archive/cs/cs106b/cs106b.1164//assn/metaAcademy.html
 * date: 8 June 2017
 */

#include <math.h>
#include "recursion.h"
#include "map.h"
#include "vector.h"
#include "set.h"
#include "gwindow.h"
#include "gobjects.h"
#include "tokenscanner.h"
#include "random.h"
using namespace std;


/*
 * Milestone-1
 * Get gcd of two numbers with steps
 * @param
 *  int:a
 *  int:b
 * @return
 *  int:gcd
*/
int gcd(int a, int b) {
    if(b==0){
        cout<<"gcd("<<a<<","<<b<<") = "<<a<<endl;
        return a;
    }
    cout<<"gcd("<<a<<","<<b<<") = "<<"gcd("<<b<<","<<a%b<<")"<<endl;
    return gcd(b,a%b);
}

/*
 * Draws a lowerTriangle
 * @param
 *  GPoint:p1
 *  GPoint:p2
 *  GPoint:p3
 * @return
 *
*/
void drawLowerTriangle(GPoint p1, GPoint p2, GPoint p3,GWindow &w){
    w.drawLine(p1,p2);
    w.drawLine(p2,p3);
    w.drawLine(p3,p1);
}

/*
 * Draws a serpinskii triangles of given order
 * @param
 *  GWindow:w
 *  int:leftX
 *  int:leftY
 *  int:size
 *  int:order
 * @return
 *
*/
void serpinskii(GWindow &w, int leftX, int leftY, int size, int order) {
    if(order == 1){
        GPoint p1(leftX,leftY);
        GPoint p2(leftX+size,leftY);
        GPoint p3(leftX+(size)/2,leftY+(size*sqrt(3)/2));
        drawLowerTriangle(p1,p2,p3,w);
        return;
    }
    else{
          serpinskii(w,leftX, leftY, size/2, order-1);
          serpinskii(w,leftX+size/2, leftY, size/2, order-1);
          serpinskii(w,leftX+size/4, leftY+(size/2*sqrt(3)/2), size/2, order-1);
    }
}

/*
 * Tells whether a pixel is in range of image
 * @param
 *  GBufferedImage:&image
 *  int:x
 *  int:y
 * @return
 *  bool
*/
bool inRange(GBufferedImage& image, int x, int y){
    return image.inBounds(x,y);
}

/*
 * Fills a given segment with new color
 * @param
 *  GBufferedImage:&image
 *  int:x
 *  int:x
 *  int:newColor
 *  int:segmentColor
 * @return
 *  int:number-of-pixelsColors
*/
int recursiveFillAndCount(GBufferedImage& image, int x, int y, int newColor, int segmentColor){
    if(!inRange(image,x,y))
        return 0;
    if(newColor == image.getRGB(x,y) || (image.getRGB(x,y)!=segmentColor))
        return 0;
    else{
        image.setRGB(x,y,newColor);
        return 1 + recursiveFillAndCount(image,x+1,y,newColor,segmentColor) + recursiveFillAndCount(image,x-1,y,newColor,segmentColor) +recursiveFillAndCount(image,x,y+1,newColor,segmentColor) +recursiveFillAndCount(image,x,y-1,newColor,segmentColor);
    }
}

int floodFill(GBufferedImage& image, int x, int y, int newColor) {
    if(!inRange(image,x,y))
        return 0;
    return recursiveFillAndCount(image,x+1,y,newColor,image.getRGB(x,y));
}

//to keep track of whether a course is printed or not
Map<string,bool> isPrinted;

/*
 * Print the intermediate personalized curriculum path
 * @param
 *  Map<string,Vector<string>>:& prereqMap
 *  string:goal
 * @return
 *
*/
void curriculumPath(Map<string, Vector<string> > & prereqMap, string goal ){
    if(prereqMap.containsKey(goal)){
        Vector<string> localDependencies = prereqMap.get(goal);
        for(int i=0; i<localDependencies.size();i++){
            string thisDependency = localDependencies.get(i);
            curriculumPath(prereqMap,thisDependency);
            if( !isPrinted.containsKey(thisDependency)){
                cout<<thisDependency<<endl;
                isPrinted.add(thisDependency,true);
            }

        }
    }

}

/*
 * Print the intermediate personalized curriculum path and goal
 * @param
 *  Map<string,Vector<string>>:& prereqMap
 *  string:goal
 * @return
 *
*/
void personalCurriculum(Map<string, Vector<string>> & prereqMap,string goal) {
    //reset printed map
    Map<string,bool> toPrint;
    isPrinted = toPrint;
    //print curriculum path
    curriculumPath(prereqMap, goal);
    //print goal
    cout<<goal<<endl;
}

/*
 * Generate a sentence from a given BNF form grammar
 * @param
 *  Map<string, Vector<string>>:& grammar
 *  string:symbol
 * @return
 *
*/
string generate(Map<string, Vector<string> > & grammar, string symbol) {
    string upto = "";
    if(grammar.containsKey(symbol)){
        Vector<string> thisExpansion = grammar.get(symbol);
        string subExp = thisExpansion.get(randomInteger(0,thisExpansion.size()-1));
        TokenScanner scanner(subExp);
        while (scanner.hasMoreTokens()) {
           string token = scanner.nextToken();
           upto += generate(grammar,token);
        }
    }else{
        return symbol;
    }
    return upto;
}
