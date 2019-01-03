using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.Linq;

public class AIBall : MonoBehaviour
{
    [SerializeField]
    [Tooltip("Just for debugging, adds some velocity during OnEnable")]
    private Vector3 initialVelocity;
    public Vector3 initialPos;

    private float timeToReset = 0.0f;
    private float maxTimeReset = 60.0f;

    [SerializeField]
    private float minVelocity = 10f;

    private Vector3 lastFrameVelocity;
    private Rigidbody rb;
    
    public Text scoreText;

    public float missedBalls;
    private int playerScore = 0;
    private int AIScore = 0;

    public int timeScale = 5;

    private int[] validChoices = new int[] { -1, 1 };
    int GetNegative()
    {
        return validChoices[Random.Range(0, validChoices.Length)];
    }

    private int[] validChoices2 = new int[] { -5, -4, -3, 3, 4, 5 };
    int GetRandomY()
    {
        return validChoices2[Random.Range(0, validChoices2.Length)];
    }

    private void OnEnable()
    {
        rb = GetComponent<Rigidbody>();
        rb.velocity = initialVelocity;
    }

    private void ResetBall()
    {
        rb.velocity = new Vector3(-10, GetRandomY(), 0);
        rb.transform.position = initialPos;
    }

    private void Update()
    {
        Time.timeScale = timeScale;

        lastFrameVelocity = rb.velocity;

        timeToReset += Time.deltaTime;
        if (timeToReset > maxTimeReset)
        {
            ResetBall();
            timeToReset = 0.0f;
        }

        if (Input.GetKeyDown("space"))
        {
            ResetBall();
        }
        
        scoreText.text = "Balls\nMissed: " + missedBalls;       
    }

    private void OnCollisionEnter(Collision collision)
    {
        Bounce(collision.contacts[0].normal);

        if (collision.collider.tag == "playerwall" || collision.collider.tag == "enemywall")
        {
            ResetBall();
        }
        if (collision.collider.tag == "enemywall")
        {
            missedBalls++;
            playerScore++;
        }        
    }

    private void Bounce(Vector3 collisionNormal)
    {
        var speed = lastFrameVelocity.magnitude;
        var direction = Vector3.Reflect(lastFrameVelocity.normalized, collisionNormal);

        //Debug.Log("Out Direction: " + direction);
        rb.velocity = direction * Mathf.Max(speed, minVelocity);
    }
}
