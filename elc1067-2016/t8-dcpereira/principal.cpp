#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <functional>
#include <queue>
#include <vector>
#include <limits>
#include <algorithm>

struct Aresta {
	int v;         // vertice destino
	float peso;    // peso da aresta
};

struct Vertice {
	int info;                   // valor do vertice
	float chave;                // chave usada no heap
	int anterior;               // quem é o anterior (Dijkstra)
	std::list<Aresta> arestas;
};

template<typename T, typename C>
struct Heap {
	std::vector<T> dado; // vetor do heap
	C compara; // funcao para comprar elementos

	// inicia o Heap com funcao de comparação
	Heap(C cmp): compara(cmp) {}

	// insere sem atualizar o heap
	void insere(T v){
		dado.push_back(v);
	}

	// remove o elemento no topo do heap
	void remove(void){
		std::pop_heap(dado.begin(), dado.end(), compara);
		dado.pop_back();
	}

	// retorna o menor elemento
	T topo(void){
		return dado.front();
	}

	// testa se heap é vazio
	bool vazio(void){
		return dado.empty();
	}

	// atualiza o heap
	void atualiza(void){
		std::make_heap(dado.begin(), dado.end(), compara);
	}
};

struct Grafo {
	int nvertices;
	int narestas;
	std::map<int, Vertice> grafo;

	void inicia(const char* arquivo){
		std::ifstream entrada {arquivo};
		if(!entrada.is_open())
			throw std::runtime_error{"ERRO nao foi possivel abrir arquivo!"};

		entrada >> nvertices;
		entrada >> narestas;

		// cria os vertices
		std::string chave, nome;
		for(auto v= 0; v < nvertices; v++){
			grafo[v] = Vertice();
			grafo[v].info = v;
		}

		int v1, v2;
		float peso;
		for(auto i= 0; i < narestas; i++){
			entrada >> v1;
			entrada >> v2;
			entrada >> peso;
			grafo[v1].arestas.push_back( Aresta{v2, peso} );
			grafo[v2].arestas.push_back( Aresta{v1, peso} );
		}
		entrada.close();
	}

	void dijkstra(const int fonte) {
		// TODO implementar função
		auto ith = grafo.begin();
		while(ith != grafo.end()){
			ith->second.chave = std::numeric_limits<float>::infinity();
			ith++;
		}

		auto compara = [](Vertice* v1, Vertice* v2) {return v1->chave > v2->chave;};
		Heap<Vertice*, decltype(compara)> heap(compara);

		grafo[fonte].chave = 0;

		for(auto& i : grafo){
			heap.insere(&i.second);
		}
		heap.atualiza();

		while(!heap.vazio()){
			Vertice * u = heap.topo();
			heap.remove();
			for(const auto& v : (*u).arestas){
				if(grafo[v.v].chave > (*u).chave + v.peso){
					grafo[v.v].anterior = (*u).info;
					grafo[v.v].chave = (*u).chave + v.peso;
					heap.atualiza();
				}
			}
		}
	}

	void imprime(void){
		for(const auto& v: grafo){
			std::cout << v.first << " -> ";
			for(const auto& a: v.second.arestas){
				std::cout << a.v << "(" << a.peso << ") ";
			}
			std::cout << std::endl;
		}
	}

	void caminho_minimo(int u, int v){
		if(u == v){
			std::cout << grafo[u].info << " ";
			return;
		}

		if(grafo[v].anterior == -1){
			std::cout << "ERRO: caminho nao existe!" << std::endl;
		} else {
			caminho_minimo( u, grafo[v].anterior );
			std::cout << grafo[v].info << " ";
		}
	}

	void teste(int u, int v){
		std::cout << "Gerando caminho mínimo por Dijkstra (fonte: "
			<< u << ")" << std::endl;
		dijkstra(u);
		std::cout << "Caminho mínimo entre u e v (" << u << " -> " <<
			v << ")" << std::endl;
		caminho_minimo(u, v);
		std::cout << std::endl;
	}
};

int main(int argc, char **argv){
	Grafo g;

	if(argc > 1) {
		g.inicia(argv[1]);
		std::cout << "Grafo de entrada:" << std::endl;
		g.imprime();
		std::cout << std::endl;
		g.teste(5, 6);
		g.teste(4, 3);
		g.teste(4, 6);
	}

	return 0;
}
