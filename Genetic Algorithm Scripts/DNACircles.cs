using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DNACircles : MonoBehaviour
{
    //Gene information about color and scale
    public float r;
    public float g;
    public float b;
    public float scale;
    
    bool dead = false;

    SpriteRenderer sRenderer;
    //Collider2D circleCollider;

    private void Start()
    {
        //Initialize all components
        sRenderer = GetComponent<SpriteRenderer>();
        //circleCollider = GetComponent<Collider2D>();
        sRenderer.color = new Color(r, g, b);
        this.transform.localScale = new Vector3(scale, scale, scale);
    }
}
