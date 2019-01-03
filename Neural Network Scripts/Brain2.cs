using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Brain2 : MonoBehaviour {

    public GameObject paddle;
    public GameObject ball;
    // public bool human = false;
    //public string backwallTag = "backwallr";
    //public Text score;
    Rigidbody brb;
    float yvel;
    float paddleMinY = -3.19f;
    float paddleMaxY = 3.9f;
    float paddleMaxSpeed = 15;
    public float numSaved = 0;
    public float numMissed = 0;

    public float learningRate = 0.1f;

    public Text learningRateText;

    ANN ann;

    void Start()
    {
        ann = new ANN(6, 1, 1, 4, learningRate); //6 inputs, 1 output, 1 hidden layer, 4 neurons inside hidden layer
        brb = ball.GetComponent<Rigidbody>();
    }


    List<double> Run(double bx, double by, double bvx, double bvy, double px, double py, double pv, bool train)
    {
        List<double> inputs = new List<double>();
        List<double> outputs = new List<double>();
        inputs.Add(bx);
        inputs.Add(by);
        inputs.Add(bvx);
        inputs.Add(bvy);
        inputs.Add(px);
        inputs.Add(py);
        outputs.Add(pv);
        if (train)
            return (ann.Train(inputs, outputs));
        else
            return (ann.CalcOutput(inputs, outputs));
    }

    // Update is called once per frame
    void Update()
    {
        learningRateText.text = "Learning\nRate:   " + learningRate;
        
        float posy = Mathf.Clamp(paddle.transform.position.y + (yvel * Time.deltaTime * paddleMaxSpeed), 6.34f, 13.37f);
        paddle.transform.position = new Vector3(paddle.transform.position.x, posy, paddle.transform.position.z);
        List<double> output = new List<double>();
        int layerMask = 1 << 12;
        RaycastHit hit;
        Physics.Raycast(ball.transform.position, brb.velocity, out hit, 1000, layerMask);
        
        if (hit.collider != null && hit.collider.gameObject.tag == "tops") //reflect off top
        {
            Vector3 reflection = Vector3.Reflect(brb.velocity, hit.normal);
            Physics.Raycast(hit.point, reflection, out hit, 1000, layerMask);
        }
        if (hit.collider != null && hit.collider.gameObject.tag == "enemywall")
        {
            float dy = (hit.point.y - paddle.transform.position.y);

            output = Run(ball.transform.position.x,
                            ball.transform.position.y,
                            brb.velocity.x, brb.velocity.y,
                            paddle.transform.position.x,
                            paddle.transform.position.y,
                            dy, true);
            yvel = (float)output[0];

        }
        
        else
            yvel = 0;
        
    }
}

