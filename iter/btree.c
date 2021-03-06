/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bst_node_t *help_node = tree;
  while ( help_node != NULL ) {
    if ( help_node->key == key) {
      *value = tree->value;
      return true;
    }
    else if ( key < help_node->key ) {
      help_node = help_node->left;
    }
    else if ( key > help_node->key) {
      help_node = help_node->right;
    }
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *help_node = *tree;
  bst_node_t *parent = NULL;
  while ( help_node != NULL) {
    if ( help_node->key == key ) {
      help_node->value = value;
      return;
    }
    else if ( key < help_node->key ) {
      parent = help_node;
      help_node = help_node->left;
    }
    else if ( key > help_node->key) {
      parent = help_node;
      help_node = help_node->right;
    }
    
  }
  bst_node_t *new_node = malloc(sizeof(bst_node_t));
  new_node->key = key;
  new_node->value = value;
  new_node->right = NULL;
  new_node->left = NULL;
  if ( parent == NULL) {
    *tree = new_node;
  }
  else {
    if ( parent->key > key ) {
      parent->left = new_node;
    }
    else {
      parent->right = new_node;
    }
  }

}



/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *help_node = *tree;
  bst_node_t *parent = NULL;

  while ( help_node->right != NULL ) {
    parent = help_node;
    help_node = help_node->right;
  }
  target->key = help_node->key;
  target->value = help_node->value;
    if ( parent == NULL ) {
    *tree = help_node->left;
   }
   else {
     parent->right = help_node->left;
   }
  

  free(help_node);

}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t *help_node = *tree;
  bst_node_t *parent = NULL;

  while ( help_node != NULL) {
    if ( help_node->key == key ) {
      if ( help_node->left != NULL && help_node->right != NULL) {
        bst_replace_by_rightmost(help_node, tree);
        return;
      }
      else if ( help_node->right == NULL && help_node->left != NULL) {
        if ( parent == NULL ) {
          *tree = help_node->left;
        }
        else if ( help_node == parent->left ) {
          parent->left = help_node->left;
        }
        else if (help_node == parent->right) {
          parent->right = help_node->left;
        }
      }
      else if ( help_node->right != NULL && help_node->left == NULL) {
        if ( parent == NULL ) {
          *tree = help_node->right;
        }
        else if ( help_node == parent->left ) {
          parent->left = help_node->right;
        }
        else if (help_node == parent->right) {
          parent->right = help_node->right;
        }
      }
      else {
        if ( parent == NULL ) {
          *tree = NULL;
        }
        else if ( help_node == parent->left ) {
          parent->left = NULL;
        }
        else if (help_node == parent->right) {
          parent->right = NULL;
        }
      }
      free(help_node);
      help_node = NULL;
      return;
    }
    else {
      if ( help_node->key > key) {
        parent = help_node;
        help_node = help_node->left;
      }
      else if ( help_node->key < key ) {
        parent = help_node;
        help_node = help_node->right;
      }
    }
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  stack_bst_init(stack);
  do {
    if ( *tree == NULL ) {
      if ( !stack_bst_empty(stack) ) {
        *tree = stack_bst_top(stack);
        stack_bst_pop(stack);
      }
    }
    else {
      if ( (*tree)->right != NULL) {
        stack_bst_push(stack,(*tree)->right);
      }
      bst_node_t *help_node = *tree;
      *tree = (*tree)->left;
      free(help_node);
    }
  } while ((*tree != NULL) || (!stack_bst_empty(stack)));
  free(stack);
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while ( tree != NULL) {
    stack_bst_push(to_visit, tree);
    bst_print_node(tree);
    tree = tree->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  stack_bst_init(stack);
  bst_leftmost_preorder(tree, stack);
  while (!stack_bst_empty(stack)) {
    tree = stack_bst_top(stack);
    stack_bst_pop(stack);
    bst_leftmost_preorder(tree->right, stack);
  }
  free(stack);
}
/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while ( tree != NULL ) {
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  stack_bst_init(stack);
  bst_leftmost_inorder(tree, stack);
  while ( !stack_bst_empty(stack)) {
    tree = stack_bst_top(stack);
    stack_bst_pop(stack);
    bst_print_node(tree);
    bst_leftmost_inorder(tree->right, stack);
  }
  free(stack);
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
                            
  while ( tree != NULL ) {
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
    tree = tree->left;
  }                       
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  stack_bst_init(stack);
  stack_bool_t *stack_bool = malloc(sizeof(stack_bool_t));
  stack_bool_init(stack_bool);
  bool left;
  bst_leftmost_postorder(tree, stack, stack_bool);
  while ( !stack_bst_empty(stack)) {
    tree = stack_bst_top(stack);
    left = stack_bool_top(stack_bool);
    stack_bool_pop(stack_bool);
    if ( left ) {
      stack_bool_push(stack_bool, false);
      bst_leftmost_postorder(tree->right, stack, stack_bool);
    }
    else {
      stack_bst_pop(stack);
      bst_print_node(tree);
    }
  }
  free(stack);
  free(stack_bool);
}
