using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using System.Linq;
using UnityEngine.UI;

public class Circles : MonoBehaviour
{

    public GameObject circlePrefab;
    List<GameObject> populationList = new List<GameObject>();
    int timeNextGeneration = 3;
    int generationNum = 1;
    public int columns = 12;
    int rows = 6;

    [Range(-5, 5)]
    public float rValue, gValue, bValue;

    public int mutationRate;
    public bool scaleTrue = true;

    public static int counter = 0;
    public static float elapsed = 0;

    [Header("UI Components")]
    public Slider rSlider;
    public Slider gSlider;
    public Slider bSlider;
    public Slider mutationSlider;
    public Text generationText;
    public Text timeText;
    public Text populationText;
    public Text mutationRateText;
    public Text rValueText;
    public Text gValueText;
    public Text bValueText;
    public Button resetButton;
    public Button pauseButton;


    float waitForInit = 0;

    private bool paused = false;
    private Vector3[] points;
    private Vector3[] points2;
    Coroutine coroutineStarted;

    

    private IEnumerator Generate()
    {
        WaitForSeconds wait = new WaitForSeconds(0.05f);
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                GameObject go = Instantiate(circlePrefab, new Vector3(j - 5.5f, i - 3), Quaternion.identity);
                go.GetComponent<DNACircles>().r = Random.Range(0.0f, 1.0f);
                go.GetComponent<DNACircles>().g = Random.Range(0.0f, 1.0f);
                go.GetComponent<DNACircles>().b = Random.Range(0.0f, 1.0f);
                go.GetComponent<DNACircles>().scale = Random.Range(0.1f, 0.5f);
                populationList.Add(go);
                yield return wait;
            }
        }
    }

    public void FlipBool()
    {
        scaleTrue = !scaleTrue; 
    }

    void ResetScene()
    {
        SceneManager.LoadSceneAsync(SceneManager.GetActiveScene().buildIndex);
    }

    void PauseScene()
    {
        if(paused == false) { Time.timeScale = 0; paused = true; }
        else if(paused == true) { Time.timeScale = 1; paused = false; }
    }

    GameObject Breed(GameObject parent1, GameObject parent2)
    {
        Vector3 pos = new Vector3(0, 0, 0);
        if (counter >= rows * columns) { counter = 0; }

        //Place the offspring in the correct grid position
        if (counter < columns) { pos = new Vector3(counter - 5.5f, -3, 0); }
        else if (counter >= columns && counter < columns * 2) { pos = new Vector3((counter - 5.5f) - columns, -2, 0); }
        else if (counter >= columns * 2 && counter < columns * 3) { pos = new Vector3((counter - 5.5f) - columns * 2, -1, 0); }
        else if (counter >= columns * 3 && counter < columns * 4) { pos = new Vector3((counter - 5.5f) - columns * 3, 0, 0); }
        else if (counter >= columns * 4 && counter < columns * 5) { pos = new Vector3((counter - 5.5f) - columns * 4, 1, 0); }
        else if (counter >= columns * 5 && counter < columns * 6) { pos = new Vector3((counter - 5.5f) - columns * 5, 2, 0); }

        GameObject offspring = Instantiate(circlePrefab, pos, Quaternion.identity);
        DNACircles dna1 = parent1.GetComponent<DNACircles>();
        DNACircles dna2 = parent2.GetComponent<DNACircles>();

        //swap parent DNA
        //-----------------------------------------------------------------------
        //THIS IS THE FUNDAMENTAL BASIC OPERATION THAT HAPPENS IN ALL GENETIC ALGORITHMS
        //We run through all values we have, all genes stored, and we randomly swap them between the parents
        //-----------------------------------------------------------------------
        if (Random.Range(1, 100) > mutationRate)
        {
            offspring.GetComponent<DNACircles>().r = Random.Range(0, 10) < 5 ? dna1.r : dna2.r; 
            offspring.GetComponent<DNACircles>().g = Random.Range(0, 10) < 5 ? dna1.g : dna2.g;
            offspring.GetComponent<DNACircles>().b = Random.Range(0, 10) < 5 ? dna1.b : dna2.b;
            offspring.GetComponent<DNACircles>().scale = Random.Range(0, 10) < 5 ? dna1.scale : dna2.scale;
        }
        else
        {
            offspring.GetComponent<DNACircles>().r = Random.Range(0.0f, 1.0f);
            offspring.GetComponent<DNACircles>().g = Random.Range(0.0f, 1.0f);
            offspring.GetComponent<DNACircles>().b = Random.Range(0.0f, 1.0f);
            offspring.GetComponent<DNACircles>().scale = Random.Range(0.1f, 0.5f);
        }
        counter += 1;
        return offspring;
    }

    void NewPopulation()
    {
        List<GameObject> sortedList;
        
        //Scale true;
        if(scaleTrue == true)
        {
            //get rid of unfit individuals
            sortedList = populationList.OrderBy(o => o.GetComponent<DNACircles>().r * rValue + (o.GetComponent<DNACircles>().g) * gValue + (o.GetComponent<DNACircles>().b) * bValue + o.GetComponent<DNACircles>().scale).ToList(); 
        }
        else
        {
            sortedList = populationList.OrderBy(o => o.GetComponent<DNACircles>().r * rValue + (o.GetComponent<DNACircles>().g) * gValue + (o.GetComponent<DNACircles>().b) * bValue - o.GetComponent<DNACircles>().scale).ToList(); 
        }
        
        populationList.Clear();

        //breed upper half of sorted list
        for (int i = (int)(sortedList.Count / 2.0f) - 1; i < sortedList.Count - 1; i++)
        {
            populationList.Add(Breed(sortedList[i], sortedList[i + 1]));
            populationList.Add(Breed(sortedList[i + 1], sortedList[i]));
        }

        //destroy all parents and previous population
        for (int i = 0; i < sortedList.Count; i++)
        {
            Destroy(sortedList[i]);
        }
        generationNum++;
    }

    // Use this for initialization
    void Start()
    {
        StartCoroutine(Generate());
        resetButton.onClick.AddListener(ResetScene);
        pauseButton.onClick.AddListener(PauseScene);
    }

    // Update is called once per frame
    void Update()
    {
        elapsed += Time.deltaTime;
        waitForInit += Time.deltaTime;
        if(waitForInit > 7)
        {
            if (elapsed > timeNextGeneration)
            {
                NewPopulation();
                elapsed = 0;
            }
        }

        rValue = rSlider.value;
        gValue = gSlider.value;
        bValue = bSlider.value;
        mutationRate = (int)mutationSlider.value;
        rValueText.text = rValue.ToString("F2");
        gValueText.text = gValue.ToString("F2");
        bValueText.text = bValue.ToString("F2");
        
        generationText.text = "Generation: " + generationNum;
        timeText.text = "Time: " + (int)elapsed;
        populationText.text = "Population: " + populationList.Count;
        mutationRateText.text = "Mutation Rate: " + mutationRate;
    }
}
