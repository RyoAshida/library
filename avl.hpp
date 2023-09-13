template<typename T>
class AVLTree {
    class Node {
    public:
        T data;
        int height;
        int diff; // (child_Lの高さ) - (child_Rの高さ)
        int size;
        Node* child_L;
        Node* child_R;
        Node(T data) {
            this->data = data;
            this->height = 0;
            this->diff = 0;
            this->size = 1;
            this->child_L = nullptr;
            this->child_R = nullptr;
        }

        void adjast_params() {
            int height_L, height_R, size_L, size_R;
            if (this->child_L == nullptr) {
                height_L = size_L = 0;
            } else {
                height_L = this->child_L->height;
                size_L = this->child_L->size;
            }
            if (this->child_R == nullptr) {
                height_R = size_R = 0;
            } else {
                height_R = this->child_R->height;
                size_R = this->child_R->size;
            }
            this->height = max(height_L, height_R) + 1;
            this->diff = height_L - height_R;
            this->size = size_L + size_R + 1;
        }
    };

    Node* root = nullptr;

    // (l v (rl rv rr)) -> ((l v rl) rv rr)
    Node* rotate_left(Node* node) {
        Node* ret = node->child_R;
        node->child_R = node->child_R->child_L;
        ret->child_L = node;

        node->adjast_params();
        return ret;
    }

    // ((ll lv lr) v r) -> (ll lv (lr v r))
    Node* rotate_right(Node* node) {
        Node* ret = node->child_L;
        node->child_L = node->child_L->child_R;
        ret->child_R = node;

        node->adjast_params();
        return ret;
    }

    // ((ll lv (lrl lrv lrr)) v r) -> ((ll lv lrl) lrv (lrr v r))
    Node* rotate_left_double(Node* node) {
        Node* ret = node->child_L->child_R;
        Node* node_L = node->child_L;
        node->child_L = node->child_L->child_R->child_R;
        node_L->child_R = node_L->child_R->child_L;
        ret->child_R = node;
        ret->child_L = node_L;

        node->adjast_params();
        node_L->adjast_params();
        return ret;
    }
    
    // (l v ((rll rlv rlr) rv rr)) -> ((l v rll) rlv (rlr rv rr))
    Node* rotate_right_double(Node* node) {
        Node* ret = node->child_R->child_L;
        Node* node_R = node->child_R;
        node->child_R = node->child_R->child_L->child_L;
        node_R->child_L = node_R->child_L->child_R;
        ret->child_L = node;
        ret->child_R = node_R;

        node->adjast_params();
        node_R->adjast_params();
        return ret;
    }


    bool find(Node* node, T data) {
        if (node == nullptr) return false;
        if (node->data == data) return true;
        return (node->data >= data ? find(node->child_L, data) : find(node->child_R, data));
    }

    Node* insert(Node* node, T data) {
        if (node == nullptr) {
            return new Node(data);
        }
        
        if (node->data >= data) {
            int prev_height = (node->child_L == nullptr ? 0 : node->child_L->height);
            node->child_L = insert(node->child_L, data);
            if (node->child_L->height > prev_height && // 左部分木の高さが1増加
                node->diff > 0) {                      // 元々左部分木の方が高い
                node = (node->child_L->diff > 0 ? rotate_right(node) : rotate_left_double(node));
            }
        } else {
            int prev_height = (node->child_R == nullptr ? 0 : node->child_R->height);
            node->child_R = insert(node->child_R, data);
            if (node->child_R->height > prev_height && // 右部分木の高さが1増加
                node->diff < 0) {                      // 元々右部分木の方が高い
                node = (node->child_R->diff < 0 ? rotate_left(node) : rotate_right_double(node));
            }
        }
        node->adjast_params();

        return node;
    }

    T get_max(Node* node) {
        if (node->child_R == nullptr) return node->data;
        return get_max(node->child_R);
    }

    Node* remove(Node* node, T data) {
        if (node == nullptr) {
            return nullptr;
        }
        
        if (node->data == data) {
            if (node->child_L == nullptr) return node->child_R;
            if (node->child_R == nullptr) return node->child_L;
            // 左部分木の最大値で書き換えて、左部分木の最大値を削除
            T left_max = get_max(node->child_L);
            node->data = left_max;
            data = left_max;
        }
        
        if (node->data >= data) {
            if (node->child_L == nullptr) return node;
            int prev_height = node->child_L->height;
            node->child_L = remove(node->child_L, data);
            int cur_height = (node->child_L == nullptr ? 0 : node->child_L->height);
            if (cur_height < prev_height && // 左部分木の高さが1減少
                node->diff < 0) {           // 元々左部分木の方が低い
                node = (node->child_R->diff <= 0 ? rotate_left(node) : rotate_right_double(node));
            }
        } else {
            if (node->child_R == nullptr) return node;
            int prev_height = node->child_R->height;
            node->child_R = remove(node->child_R, data);
            int cur_height = (node->child_R == nullptr ? 0 : node->child_R->height);
            if (cur_height < prev_height && // 右部分木の高さが1減少
                node->diff > 0) {           // 元々右部分木の方が低い
                node = (node->child_L->diff >= 0 ? rotate_right(node) : rotate_left_double(node));
            }
        }
        node->adjast_params();

        return node;
    }

    T at(Node* node, int k) {
        int size_L = (node->child_L == nullptr ? 0 : node->child_L->size);
        if (size_L == k) return node->data;
        return (size_L > k ? at(node->child_L, k) : at(node->child_R, k-size_L-1));
    }

public:
    int height() {
        return (this->root == nullptr ? 0 : this->root->height);
    }

    int size() {
        return (this->root == nullptr ? 0 : this->root->size);
    }

    T at(int k) {
        if (this->root == nullptr || this->root->size <= k) throw "out of bounds exception.";
        return at(this->root, k);
    }

    bool find(T data) {
        return find(this->root, data);
    }

    void insert(T data) {
        this->root = insert(this->root, data);
    }

    void remove(T data) {
        this->root = remove(this->root, data);
    }

    void remove_at(int k) {
        T data = at(k);
        remove(data);
    }

};
