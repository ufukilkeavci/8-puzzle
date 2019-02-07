#include<stdlib.h>
#include<stdio.h>
#include<time.h>



typedef struct node{
    int board[9];
    int active, manhattan;
    struct node * left, * right, * up, * down, * parent;
} node;

void move_board(node *, node *, int, int);
int min_node(node *, node *, node *, node *);
void calculate_children(node *);
void manhattan_distance(node *);
void check_parents(node *);
int is_equal(node *, node *);
node * move(node *);
void print_board(node *);

void calculate_children(node * position){
    int i, zero_location;
    
    for(i=0; i<9; i++){
        if(position->board[i] == 0){
            zero_location = i;
            break;
        }
    }

    position->left = (node *)malloc(sizeof(node));
    position->up = (node *)malloc(sizeof(node));
    position->right = (node *)malloc(sizeof(node));
    position->down = (node *)malloc(sizeof(node));

    position->left->parent = position;
    position->up->parent = position;
    position->right->parent = position;
    position->down->parent = position; 

    position->left->active = 1;
    position->up->active = 1;
    position->right->active = 1;
    position->down->active = 1;

    move_board(position, position->left, 1, zero_location);
    move_board(position, position->up, 2, zero_location);
    move_board(position, position->right, 3, zero_location);
    move_board(position, position->down, 4, zero_location);

    check_parents(position->left);
    check_parents(position->up);
    check_parents(position->right);
    check_parents(position->down);

    manhattan_distance(position->left);
    manhattan_distance(position->up);
    manhattan_distance(position->right);
    manhattan_distance(position->down);

}

node * move(node * position){
    print_board(position);
    if(position->manhattan == 0){
        return position;
    }
    else{
        int min = min_node(position->left, position->up, position->right, position->down);
        switch(min){
            case 1:
                return position->left;
                break;
            case 2:
                return position->up;
                break;
            case 3:
                return position->right;
                break;
            case 4:
                return position->down;
                break;
        }
    }
}

void print_board(node * pos){
    printf("\n %d %d %d\n %d %d %d\n %d %d %d\n", pos->board[0], pos->board[1], pos->board[2], pos->board[3], pos->board[4], pos->board[5], pos->board[6], pos->board[7], pos->board[8]);
}

int is_equal(node * n1, node * n2){
    int i;
    for(i=0; i<9; i++){
        if(n1->board[i] != n2->board[i]){
            return 0;
        }
    }
    return 1;
}

void check_parents(node * position){
    node * parent = position->parent;
    while(parent != NULL){
        if(is_equal(position, parent)){
            position->active = 0;
            return;
        }
        parent = parent->parent;
    }
}

void manhattan_distance(node * position){
    
    if(position->active == 0){
        position->manhattan = 1000;
        return;
    }else if(position->active == 2){
        position->manhattan = 5000;
        return;
    }
    
    int manhattan = 0;
    int i;
    for(i=0; i<9; i++){
        manhattan += abs((position->board[i]%3)-(i%3)) + abs((position->board[i]/3)-(i/3)); 
    }
    position->manhattan = manhattan;
}

//direction: 1-Left, 2-Up, 3-Right, 4-Down
void move_board(node * current, node * child, int direction, int zero_location){
    
    int move_location;
    
    switch(zero_location){
        case 0:
            if(direction == 1 || direction == 2){
                child->active = 2;
                return;
            }
            break;
        case 1:
        if(direction == 2){
                child->active = 2;
                return;
            }
            break;
        case 2:
        if(direction == 2 || direction == 3){
                child->active = 2;
                return;
            }
            break;
        case 3:
        if(direction == 1){
                child->active = 2;
                return;
            }
            break;
        case 4:
            break;
        case 5:
        if(direction == 3){
                child->active = 2;
                return;
            }
            break;
        case 6:
        if(direction == 1 || direction == 4){
                child->active = 2;
                return;
            }
            break;
        case 7:
        if(direction == 4){
                child->active = 2;
                return;
            }
            break;
        case 8:
            if(direction == 3 || direction == 4){
                child->active = 2;
                return;
            }
            break;
    }

    switch(direction){
        case 1:
            move_location = zero_location - 1;
            break;
        case 2:
            move_location = zero_location - 3;
            break;
        case 3:
            move_location = zero_location + 1;
            break;
        case 4:
            move_location = zero_location + 3;
            break;
    }
    
    /*switch(direction){
        case 1:
            if(((zero_location/3) != (zero_location-1)/3) || zero_location - 1 < 0){
                child->active = 0;
                return;
            }
            move_location = zero_location - 1;
            break;
        case 2:
            if(((zero_location%3) != (zero_location-3%(3))) || zero_location - 3 < 0){
                child->active = 0;
                return;
            }
            move_location = zero_location - 3;
            break;
        case 3:
            if((zero_location/3) != (zero_location+1)/3){
                child->active = 0;
                return;
            }
            move_location = zero_location + 1;
            break;
        case 4:
            if(((zero_location%3) != (zero_location+3%(3))) || zero_location + 3 > 8){
                child->active = 0;
                return;
            }
            move_location = zero_location + 3;
            break;
    }*/
    
    int i;
    for(i=0; i<9; i++){
        if(i == zero_location){
            child->board[i] = current->board[move_location];
        }else if(i == move_location){
            child->board[i] = 0;
        }else{
            child->board[i] = current->board[i];
        }
    }
    return;
}

int min2(int a, int b){
    if(a<b)
        return a;
    return b;
}

int min4(int a, int b, int c, int d){
    int x = min2(a, b);
    int y = min2(c, d);
    return min2(x, y);
}

int min_node(node * c1, node * c2, node * c3, node * c4){
    int min = min4(c1->manhattan, c2->manhattan, c3->manhattan, c4->manhattan);
    int mins = 0;
    int random_select = rand();
    if(min == c1->manhattan){
        mins += 1;
    }
    if(min == c2->manhattan){
        mins += 2;
    }
    if(min == c3->manhattan){
        mins += 4;
    }
    if(min == c4->manhattan){
        mins += 8;
    }
    switch(mins){
    	case 1:
    		return 1;
    		break;
    	case 2:
    		return 2;
    		break;
		case 3:
			random_select = random_select % 2;
			if(random_select == 0){
				return 1;
			}
			return 2;
			break;
		case 4:
			return 3;
			break;
		case 5:
			random_select = random_select % 2;
			if(random_select == 0){
				return 1;
			}
			return 3;
			break;
		case 6:
			random_select = random_select % 2;
			if(random_select == 0){
				return 3;
			}
			return 2;
			break;
		case 7:
			random_select = random_select % 3;
			if(random_select == 0){
				return 1;
			}else if(random_select == 1){
				return 2;
			}
			return 3;
			break;
		case 8:
			return 4;
			break;
		case 9:
			random_select = random_select % 2;
			if(random_select == 0){
				return 1;
			}
			return 4;
			break;
		case 10:
			random_select = random_select % 2;
			if(random_select == 0){
				return 4;
			}
			return 2;
			break;
		case 11:
			random_select = random_select % 3;
			if(random_select == 0){
				return 1;
			}else if(random_select == 1){
				return 2;
			}
			return 4;
			break;
		case 12:
			random_select = random_select % 2;
			if(random_select == 0){
				return 4;
			}
			return 3;
			break;
		case 13:
			random_select = random_select % 3;
			if(random_select == 0){
				return 1;
			}else if(random_select == 1){
				return 4;
			}
			return 3;
			break;
		case 14:
			random_select = random_select % 3;
			if(random_select == 0){
				return 4;
			}else if(random_select == 1){
				return 2;
			}
			return 3;
			break;
		case 15:
			random_select = random_select % 4;
			if(random_select == 0){
				return 1;
			}else if(random_select == 1){
				return 2;
			}else if(random_select == 2){
				return 4;
			}
			return 3;
			break;		
	}
    
    /*if(c1->manhattan < c2->manhattan && c1->manhattan < c3->manhattan && c1->manhattan < c4->manhattan)
        return 1;
    else if(c2->manhattan < c1->manhattan && c2->manhattan < c3->manhattan && c2->manhattan < c4->manhattan)
        return 2;
    else if(c3->manhattan < c2->manhattan && c3->manhattan < c1->manhattan && c3->manhattan < c4->manhattan)
        return 3;
    else if(c4->manhattan < c2->manhattan && c4->manhattan < c3->manhattan && c4->manhattan < c1->manhattan)
        return 4;
    else{
        if(c1->manhattan < 500)
            return 1;
        else if(c2->manhattan < 500)
            return 2;
        else if(c3->manhattan < 500)
            return 3;
        else if(c4->manhattan < 500)
            return 4;
        else
    }*/
}

void write_board(FILE * f, node * n){
    int i, j;
    for(i=0; i<3; i++){
        for(j=0; j<3; j++){
            fprintf(f, "%d ", n->board[3*i+j]);
        }
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
}

int main(){

    int board[9] = {3,4,6,2,8,5,1,7,0};
    int i, count = 0;
    FILE * output;
    output = fopen("output.txt","w");

    node * current_node;
    current_node = (node *)malloc(sizeof(node));
    current_node->parent = NULL;
    current_node->active = 1;

    int t = time(NULL);

    for(i=0; i<9; i++){
        current_node->board[i] = board[i];
    }

    calculate_children(current_node);
    manhattan_distance(current_node);

    while(current_node->manhattan != 0){
        count++;
        current_node = move(current_node);
        calculate_children(current_node);
        write_board(output, current_node);
        if(count % 50 == 0){
            fprintf(output, "%d\n\n", count);
        }
    }
    fprintf(output, "Puzzle solved in %d steps in %d ms.\n", count, time(NULL)-t);
    fclose(output);
}
