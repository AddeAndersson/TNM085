//
// Pseudocode: BorderCollision & ballToBallCollision
//
//

/*-----------------------------/
/        BorderCollision       /
/-----------------------------*/


void ballToBorderCollision( glm::vec2 ballPositions[], glm::vec2 ballVelocities[] ){

    for(int i = 0; i < 16; i++){

        // Separating factor sep. Used in MatLab to avoid balls sticking to
        // eachother. Maybe not needed in c++/openGL
        sep = 10^-3;

        // Table properties
        xMaxLenghtTable = 2.13;
        xMinLenghtTable = 0;

        yMaxLenghtTable = 1.065;
        yMinLenghtTable = 0;

        // Ball properties, r = radius;
        r = 0.0286;

        //check if particules collided with the walls horizontally (x-direction)
        if ( ballPositions[i].x + r >= xMaxLenghtTable - sep || ballPositions[i].x - r <=  xMinLenghtTable + sep ){

            // Formula to calculate velocity drop.
            ballVelocities[i].x = sqrt(0.75*(ballVelocities[i].x )^2);
            // New direction
            ballVelocities[i].x = -ballVelocities[i].x;

        }

        // check if particules collided with the walls vertically (y-direction)
        if( ballPositions[i].y + r >= yMaxLengthTable - sep || ballPositions[i].y  - r <= yMinLengthTable + sep ){

            // Formula to calculate velocity drop.
            ballVelocities[i].y = sqrt(0.75*(ballVelocities[i].y )^2);

            // New direction
            ballVelocities[i].y  = -ballVelocities[i].y;
        }
    }
}




/*----------------------------------/
/        ballToBallCollision        /
/----------------------------------*/

void ballToBallCollision( glm::vec2 ballPositions[], glm::vec2 ballVelocities[] ){
    // Auxiliary variables
    glm::vec2 temp;

    // temp variables to store constants
    float C1,C2;
    float massWhiteBall = 0.170;
    float massRestOfBalls = 0.165;
    for(int  i = 0; i < numberOfballs-1 ; i++){

    //
    if( i == 0){
        m1 = massWhiteBall;
        m2 = massRestOfBalls;
    }else{
        m1 = m2 = mRestOfBalls;
    }

        //temp = ballPositions[i] - ballPositions[i+1];
    //norm = sqrt(temp.x^2 + temp.y^2);

    norm = distance(ballPositions[i], ballPositions[i+1]);

    // Separating factor sepa. Used in MatLab to avoid overlap. Maybe not needed in c++/openGL
    sep = 10^-3;

    // Check for collision
    if( ( norm  <= (2*r + sep) ){

        // compute new direction
        C1 = ( 2*m2/(m1+m2) )*dot( (ballVelocities[i]-ballVelocities[i+1]) , (ballPositions[i] - ballVPositions[i+1]) );
        Cx = ( 2*m1/(m1+m2) )*dot( (ballVelocities[i+1]-ballVelocities[i]) , (ballPositions[i+1] - ballVPositions[i]) );

        // Update velocities

        ballVelocities[i] = ballVelocities[i] - C1*(ballPositions[i] - ballVPositions[i+1]);
        ballVelocities[i+1] = ballVelocities[i+1] - C1*(ballPositions[i+1] - ballVPositions[i]);
    }
}


/*----------------------------------/
/             Friction              /
/----------------------------------*/

//New position
ballPositions =  ballPositions + ballVelocities*time;
